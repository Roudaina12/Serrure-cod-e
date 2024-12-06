/*
 * Based on AnalogRead_DigitalRead example from: https://github.com/feilipu/Arduino_FreeRTOS_Library
 * Modified by: Frederic Pillon <frederic.pillon (at) st.com>
 */
#include <Arduino.h>
#include <STM32FreeRTOS.h>

// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

QueueHandle_t xQueue;
// define two Tasks for DigitalRead & AnalogRead
void Task_Keypad( void *pvParameters );
void Task_LCD_actuator( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
  if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

xQueue = xQueueCreate( 5, sizeof( char ) ); 
  // Now set up two Tasks to run independently.
  xTaskCreate(
    Task_Keypad
    ,  (const portCHAR *)"Keypad_Handler"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    Task_LCD_actuator
    ,  (const portCHAR *) "LCD_Actuator_Handler"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // start scheduler
  vTaskStartScheduler();
  Serial.println("Insufficient RAM");
  while(1);
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void Task_Keypad( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  /*
    DigitalReadSerial
    Reads a digital input on pin defined with USER_BTN, prints the result to the serial monitor

    This example code is in the public domain.
  */
 char Key; 
 portBASE_TYPE xStatus;

  for (;;) // A Task shall never return or exit.
  {
    // read the input keypad:
    

    xStatus = xQueueSendToBack( xQueue, &Key, 0 ); 

 if( xStatus != pdPASS ) 
 { 
 /* The send operation could not complete because the queue was full - 
 this must be an error as the queue should never contain more than 
 one item! */ 
 } 

 
    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the state of the button:
      Serial.print("Keypad state:");
     
      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    vTaskDelay(1);  // one tick delay in between reads for stability
  }
}

void Task_LCD_actuator( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    char Key;

  for (;;)
  {

     xStatus = xQueueReceive( xQueue, &Key, xTicksToWait ); 

 if( xStatus == pdPASS ) 
 { 
 /* Data was successfully received from the queue, print out the received 
 value. */ 
 //vPrintStringAndNumber( "Received = ", lReceivedValue ); 
 } 
 else 
 { 
 /* Data was not received from the queue even after waiting for 100ms. 
 This must be an error as the sending tasks are free running and will be 
 continuously writing to the queue. */
 //vPrintString( "Could not receive from the queue.\n" ); 
 }

     // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the value you read:
      Serial.println("update system status");

      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    vTaskDelay(1);  // one tick delay in between reads for stability
  }
}