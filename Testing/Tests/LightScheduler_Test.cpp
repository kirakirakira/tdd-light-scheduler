/*!
 * @file
 * @brief Tests for light scheduler implementation.
 */

extern "C"
{
#include "LightScheduler.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "DigitalOutputGroup_Mock.h"
#include "TimeSource_Mock.h"
#include "uassert_test.h"

TEST_GROUP(LightScheduler)
{
   LightScheduler_t scheduler;

   DigitalOutputGroup_Mock_t fakeDigitalOutputGroup;
   TimeSource_Mock_t fakeTimeSource;

   void setup()
   {
      DigitalOutputGroup_Mock_Init(&fakeDigitalOutputGroup);
      TimeSource_Mock_Init(&fakeTimeSource);

      LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   }

   void LightShouldBeTurnedOn(uint8_t which)
   {
      mock()
          .expectOneCall("Write")
          .onObject(&fakeDigitalOutputGroup)
          .withParameter("channel", which)
          .withParameter("state", true);
   }

   void LightShouldBeTurnedOff(uint8_t which)
   {
      mock()
          .expectOneCall("Write")
          .onObject(&fakeDigitalOutputGroup)
          .withParameter("channel", which)
          .withParameter("state", false);
   }

   void WhenTheTimeIs(TimeSourceTickCount_t time)
   {
      mock()
          .expectOneCall("GetTicks")
          .onObject(&fakeTimeSource)
          .andReturnValue(time);
   }

   void NothingShouldHappen()
   {
   }

   void WhenTheLightSchedulerIsRunAtTime(TimeSourceTickCount_t time)
   {
      WhenTheTimeIs(time);
      LightScheduler_Run(&scheduler);
   }

   void WhenTheLightSchedulerIsRun()
   {
      WhenTheTimeIs(0);
      LightScheduler_Run(&scheduler);
   }
};

TEST(LightScheduler, ShouldNotDoAnythingIfWeDontSetAnySchedules)
{
   NothingShouldHappen();
   WhenTheLightSchedulerIsRun();
}

TEST(LightScheduler, ShouldRunASchedule)
{
   LightScheduler_AddSchedule(&scheduler, 3, true, 12);

   NothingShouldHappen();
   WhenTheLightSchedulerIsRunAtTime(11);

   LightShouldBeTurnedOn(3);
   WhenTheLightSchedulerIsRunAtTime(12);
}

TEST(LightScheduler, ShouldRunTwoSchedules)
{
   LightScheduler_AddSchedule(&scheduler, 3, true, 12);
   LightScheduler_AddSchedule(&scheduler, 4, true, 13);

   NothingShouldHappen();
   WhenTheLightSchedulerIsRunAtTime(11);

   LightShouldBeTurnedOn(3);
   WhenTheLightSchedulerIsRunAtTime(12);

   LightShouldBeTurnedOn(4);
   WhenTheLightSchedulerIsRunAtTime(13);
}

TEST(LightScheduler, ShouldTurnOnAndOffSameLightWithTwoSchedules)
{
   LightScheduler_AddSchedule(&scheduler, 3, true, 12);
   LightScheduler_AddSchedule(&scheduler, 3, false, 13);

   NothingShouldHappen();
   WhenTheLightSchedulerIsRunAtTime(11);

   LightShouldBeTurnedOn(3);
   WhenTheLightSchedulerIsRunAtTime(12);

   LightShouldBeTurnedOff(3);
   WhenTheLightSchedulerIsRunAtTime(13);
}

TEST(LightScheduler, ShouldDoNothingAfterAddingAndRemovingSchedule)
{
   LightScheduler_AddSchedule(&scheduler, 3, true, 12);
   LightScheduler_RemoveSchedule(&scheduler, 3, true, 12);

   WhenTheLightSchedulerIsRunAtTime(12);
   NothingShouldHappen();
}

TEST(LightScheduler, ShouldDoNothingAfterRemovingScheduleThatDoesntExist)
{
   LightScheduler_RemoveSchedule(&scheduler, 3, true, 12);

   WhenTheLightSchedulerIsRunAtTime(12);
   NothingShouldHappen();
}

// TEST(LightScheduler, ShouldNotRunMoreThanMaxSchedules) {
//    LightScheduler_AddSchedule(&scheduler, 1, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 2, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 3, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 4, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 5, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 6, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 7, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 8, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 9, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 10, true, 13);
//    LightScheduler_AddSchedule(&scheduler, 11, true, 13);

//    LightShouldBeTurnedOn(1);
//    LightShouldBeTurnedOn(2);
//    LightShouldBeTurnedOn(3);
//    LightShouldBeTurnedOn(4);
//    LightShouldBeTurnedOn(5);
//    LightShouldBeTurnedOn(6);
//    LightShouldBeTurnedOn(7);
//    LightShouldBeTurnedOn(8);
//    LightShouldBeTurnedOn(9);
//    LightShouldBeTurnedOn(10);
//    LightShouldBeTurnedOn(11);

//    WhenTheLightSchedulerIsRunAtTime(13);
// }


