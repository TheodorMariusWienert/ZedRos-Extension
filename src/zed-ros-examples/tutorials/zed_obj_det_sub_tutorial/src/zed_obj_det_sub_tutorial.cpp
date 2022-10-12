///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2018, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/**
 * This tutorial demonstrates how to receive the list of detected objects from a ZED node
 * from the ZED node
 */

#include <ros/ros.h>

#include <zed_interfaces/Object.h>
#include <zed_interfaces/ObjectsStamped.h>
#include <iostream>
#include <fstream>
/**
 * Subscriber callbacks. The argument of the callback is a constant pointer to the received message
 */
double timeForDetection[2][1000];//Time from image capture 
double posData[5][300];
int idx_toStart=0;
int idx=0;
bool timeRecording=false;
bool posDataRecording=false;
ros::Time lastStamp;


void savePositionalData(){

    std::ofstream posDataFile;
     posDataFile.open ("/home/theubuntu/ZedRos_CustomDetection/YoloV5 ResultsonTest/posData_Tiago_1_6m.txt");
        
        for(int i = 0; i < (sizeof(posData[0])/sizeof(double)); ++i){
            for(int j =0;j<(sizeof(posData)/sizeof(posData[0]));j++)
            {
              if(j==0)
              {
                posDataFile<<"class: "<<std::to_string(posData[j][i])+",";
              }
              else{
                posDataFile<<std::to_string(posData[j][i])+",";
              }
                
               
            }
              posDataFile<<"\n"; 
        }
       
        posDataFile.close();
                                          
}
void saveData(){

    std::ofstream fileTimeDetect;
     fileTimeDetect.open ("/home/theubuntu/ZedRos_CustomDetection/YoloV5 ResultsonTest/Retrieve+Ingest+Message+Publish/timesForDetectionSub.txt");
        
        for(int i = 0; i < (sizeof(timeForDetection[0])/sizeof(double)); ++i){
            for(int j =0;j<(sizeof(timeForDetection)/sizeof(timeForDetection[0]));j++)
            {
                fileTimeDetect<<std::to_string(timeForDetection[j][i])+",";
               
            }
              fileTimeDetect<<"\n"; 
        }
       
        fileTimeDetect.close();
                                          
}

void objectListCallback(const zed_interfaces::ObjectsStamped::ConstPtr& msg)
{
  //ROS_INFO("***** New object list *****");
  for (int i = 0; i < msg->objects.size(); i++)
  {
    if (msg->objects[i].label_id == -1)
      continue;
     
   
    

    // ROS_INFO_STREAM(msg->objects[i].sublabel << " [" << msg->objects[i].label_id << "] - Pos. ["
    //                                       << msg->objects[i].position[0] << "," << msg->objects[i].position[1] << ","
    //                                       << msg->objects[i].position[2] << "] [m]"
    //                                       << "- Conf. " << msg->objects[i].confidence
    //                                      // << " - Tracking state: " << static_cast<int>(msg->objects[i].tracking_state)
    //                                       << " - Action state: " << static_cast<int>(msg->objects[i].action_state)
    //                                       << msg->objects[i].bounding_box_3d.corners[4]
    //                                       <<"\n"
    //                                       << "widthBB"<<widthBB<<", "
    //                                       << "heightBB"<<heightBB<<", "
    //                                       << "deepthe"<<depthBB
    //                                       <<"\n"
    //                                       <<"min Distance X: "<<min_distanceX<<", "
    //                                       <<"\n");
    if(timeRecording||posDataRecording){
    if(idx_toStart<100){
      idx_toStart++;
    }
    if(timeRecording){
       if(idx_toStart>=100 && idx<(sizeof(timeForDetection[0])/sizeof(double))){
       
        ros::Time stamp = msg->header.stamp; // Timestamp
        
        if(stamp!=lastStamp){
        ros::Time nowStamp = ros::Time::now();//each image only once
        double stamp_ms=(double(stamp.sec) + double(stamp.nsec)*1e-9)*1e3;
        double nowStamp_ms=(double(nowStamp.sec) + double(nowStamp.nsec)*1e-9)*1e3;
        double diff=nowStamp_ms-stamp_ms;
        timeForDetection[0][idx]=diff;
        timeForDetection[1][idx]=nowStamp_ms;
        idx++;
        lastStamp=stamp;
        }
        else{
        //same img timestamp, same image differnt object"
        double stamp_ms=(double(stamp.sec) + double(stamp.nsec)*1e-9)*1e3;
        ros::Time nowStamp2 = ros::Time::now();
        double nowStamp2_ms=(double(nowStamp2.sec) + double(nowStamp2.nsec)*1e-9)*1e3;
        double diff=nowStamp2_ms-stamp_ms;
        timeForDetection[0][idx-1]=diff;
        timeForDetection[1][idx-1]=nowStamp2_ms;
        }

      }
      if(idx==(sizeof(timeForDetection[0])/sizeof(double))){
      saveData();
        idx++;
      }
    }
    else{
       if(idx_toStart>=100 && idx<(sizeof(posData[0])/sizeof(double))){
        
         auto widthBB =msg->objects[i].bounding_box_3d.corners[4].kp[1]-msg->objects[i].bounding_box_3d.corners[7].kp[1];
         auto heightBB = -msg->objects[i].bounding_box_3d.corners[4].kp[2]+ msg->objects[i].bounding_box_3d.corners[0].kp[2];
        auto depthBB = -msg->objects[i].bounding_box_3d.corners[4].kp[0]+ msg->objects[i].bounding_box_3d.corners[5].kp[0];

        auto min_distanceX= fmin(msg->objects[i].bounding_box_3d.corners[4].kp[0],msg->objects[i].bounding_box_3d.corners[7].kp[0]);

   
       

        if( msg->objects[i].sublabel== "kuka") posData[0][idx]=0;
        if( msg->objects[i].sublabel== "human") posData[0][idx]=1;
        if( msg->objects[i].sublabel== "tiago") posData[0][idx]=2;
        if( msg->objects[i].sublabel== "pepper") posData[0][idx]=3;
        posData[1][idx]=widthBB;
        posData[2][idx]=heightBB;
        posData[3][idx]=depthBB;
        posData[4][idx]=min_distanceX;;
        idx++;
   
   // std::cout<<diff<<"\n";
      }
      if(idx==(sizeof(posData[0])/sizeof(double))){
       savePositionalData();
      idx++;
      }



    }
   
    }
   
  


   
  }
}


/**
 * Node main function
 */
int main(int argc, char** argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "zed_obj_det_sub_tutorial");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called imageCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber subObjList = n.subscribe("objects", 100, objectListCallback);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}
