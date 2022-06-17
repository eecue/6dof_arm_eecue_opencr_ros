#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>

#include <Servo.h>


Servo myservo[5]; 

rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

#define LED_PIN 25

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}


void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void subscription_callback_0(const void * msgin )
{  
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;
    if (msg->linear.x > 79){
      myservo[0].write(msg->linear.x);
    }
    if (msg->linear.y > -1){
      myservo[1].write(msg->linear.y);
    }
    if (msg->linear.z > -1){
      myservo[2].write(msg->linear.z);
    }
    if (msg->linear.x > -1){
      myservo[3].write(msg->angular.x);
    }
    if (msg->angular.y > -1){
      myservo[4].write(msg->angular.y);
    }
    if (msg->angular.z > -1){
      myservo[5].write(msg->angular.z);
    }
  
}

void setup() {

  delay(2000);

  myservo[0].attach(3);  // Shoulder 80 -> 180
  myservo[1].attach(5);  // Forearm Rotate 0 -> 180
  myservo[2].attach(6);  // Elbow 0 -> 180 
  myservo[3].attach(9);  // Arm Rotate 0 -> 180 
  myservo[4].attach(10); // Wrist Flex 0 -> 180
  myservo[5].attach(11); // Wrist Rotate 0 -> 180
  
  set_microros_transports();
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "eecue_6dof_arm_node", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "eecue_6dof_arm_subscriber"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback_0, ON_NEW_DATA));
}

void loop() {
  delay(100);
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
