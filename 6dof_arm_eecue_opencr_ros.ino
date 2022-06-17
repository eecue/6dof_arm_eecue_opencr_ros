#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <sensor_msgs/msg/joint_state.h>


#include <Servo.h>

#define SHOULDER_PIN 3
#define FOREARM_PIN 5
#define ELBOW_PIN 6
#define ARM_ROTATE_PIN 9
#define WRIST_FLEX_PIN 10
#define WRIST_ROTATE_PIN 11

#define ARRAY_LEN 6

#define LED_PIN 25

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

Servo shoulder;
Servo forearm;
Servo elbow;
Servo arm_rotate;
Servo wrist_flex;
Servo wrist_rotate; 

rcl_subscription_t subscriber;
sensor_msgs__msg__JointState * joint_states_msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;



void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}


void subscription_callback(const void * msgin )
{  
 	const sensor_msgs__msg__JointState * joint_states_msg = (const sensor_msgs__msg__JointState *)msgin;

    // Shoulder starts at 80
    if (joint_states_msg->position.data[0] > 79){
      shoulder.write(joint_states_msg->position.data[0]);
    }
    if (joint_states_msg->position.data[1] > -1){
      forearm.write(joint_states_msg->position.data[1]);
    }
    if (joint_states_msg->position.data[2] > -1){
      elbow.write(joint_states_msg->position.data[2]);
    }
    if (joint_states_msg->position.data[3] > -1){
      arm_rotate.write(joint_states_msg->position.data[3]);
    }
    if (joint_states_msg->position.data[4] > -1){
      wrist_flex.write(joint_states_msg->position.data[4]);
    }
    if (joint_states_msg->position.data[5] > -1){
      wrist_rotate.write(joint_states_msg->position.data[5]);
    }
  
}

void setup() {

  delay(2000);

  shoulder.attach(SHOULDER_PIN);  // Shoulder 80 -> 180
  forearm.attach(FOREARM_PIN);  // Forearm Rotate 0 -> 180
  elbow.attach(ELBOW_PIN);  // Elbow 0 -> 180 
  arm_rotate.attach(ARM_ROTATE_PIN);  // Arm Rotate 0 -> 180 
  wrist_flex.attach(WRIST_FLEX_PIN); // Wrist Flex 0 -> 180
  wrist_rotate.attach(WRIST_ROTATE_PIN); // Wrist Rotate 0 -> 180
  
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
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, joint_states_msg, JointState),
    "eecue_6dof_arm_subscriber"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &joint_states_msg, &subscription_callback, ON_NEW_DATA));
}

void loop() {
  delay(100);
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
