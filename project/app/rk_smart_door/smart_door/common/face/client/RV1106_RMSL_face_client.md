### RV1106 RMSL Face Client人脸模块通信说明

* 接口函数分为两类，1.通过client向模组发起请求，2.接收模组消息

##### 1.Client初始化/反初始化

* Client初始化

  ```c
  /*******************
  输入参数:  无
  返回参数： 0：成功
           非0：失败
  *********************/
  int face_client_init(void)
  ```

*  Client连接状态查询(非阻塞)

  ```c
  /*******************
  输入参数:  无
  返回参数： 1：已连接
           0：未连接
  *********************/
  int face_client_check_connect(void)
  ```
  
*  Client反初始化

  ```c
  /*******************
  输入参数:  无
  返回参数： 0：成功
           非0：失败
  *********************/
  int face_client_deinit(void);
  ```



##### 2. Client请求接口

* 人脸验证

  ```c
  /*******************
  输入参数: seconds: 验证的超时时间
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int set_module_face_verify(int seconds)
  ```

* 人脸注册

  ```c
  /*******************
  输入参数:  seconds: 注册的超时时间
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int set_module_face_enroll(int seconds)
  ```

* 人脸多方向模式注册

  ```c
  /*******************
  输入参数:  seconds: 注册的超时时间
  		 dir:本次注册的人脸方向，定义如下：
  		#define FACE_DIRECTION_UP       0x10       // face up
  		#define FACE_DIRECTION_DOWN     0x08       // face down
  		#define FACE_DIRECTION_LEFT     0x04       // face left
  		#define FACE_DIRECTION_RIGHT    0x02       // face right
  		#define FACE_DIRECTION_MIDDLE   0x01       // face middle
  		#define FACE_DIRECTION_UNDEFINE 0x00       // face undefine
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int set_module_face_enroll_multi_dir(int seconds, int dir);
  ```

* 清除所有人脸

  ```c
  /*******************
  输入参数:  无
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int set_module_clr_all_face(void)
  ```

* 清除单个人脸

  ```c
  /*******************
  输入参数:  无
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int set_module_clr_face_id(int face_id)
  ```

* 退出人脸检测

  ```c
  /*******************
  输入参数:  无
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int set_module_exit_face(void);
  ```




##### 3. Client模组消息通知回调


回调注册后，模组在初始化、注册、验证以及人脸状态会通过回调通知给Client。

* 人脸模组准备就绪函数注册，通知Client模组已就绪

  ```c
  /*******************
  输入参数:  回调函数
  返回参数： 无
  *********************/
  void set_module_ready_cb(module_ready_cb_t cb)
  ```

* 人脸解锁结果消息函数注册，模组处理完解锁任务主动调用

  ```c
  /*******************
  输入参数:  回调函数
	返回参数： 无
  *********************/
  void set_verify_result_cb(verity_result_cb_t cb)
  ```
  
* 人脸注册结果消息函数注册，模组处理完注册任务主动调用

  ```c
  /*******************
  输入参数:  回调函数
	返回参数： 无
  *********************/
  void set_enroll_result_cb(enroll_result_cb_t cb)
  ```

* 人脸状态消息函数注册，解锁过程中的人脸状态返回

  ```c
  /*******************
  输入参数:  回调函数
  返回参数： 无
  *********************/
  void set_face_state_cb(face_state_cb_t cb)
  ```




##### 4.注册回调及解锁、验证等定义

* 回调函数类型定义

  ```c
  //初始化回调
  typedef void (*module_ready_cb_t)(void);
  //验证结果回调：
  typedef void (*verity_result_cb_t)(int result, int usr_id);
  //注册结果回调：
  typedef void (*enroll_result_cb_t)(int result, int usr_id);
  //人脸状态回调：
  typedef void (*face_state_cb_t)(int result);
  ```

* 验证及注册结果为int类型，对应的定义如下

  ```c
   enum EN_MMI_RET {
      MR_SUCCESS                          = 0, // success
      MR_REJECTED                         = 1, // module rejected this command
      MR_ABORTED                          = 2, // algo aborted
      MR_FAILED4_CAMERA                   = 4, // camera open failed
      MR_FAILED4_UNKNOWNREASON            = 5, // UNKNOWN_ERROR
      MR_FAILED4_INVALIDPARAM             = 6, // invalid param
      MR_FAILED4_NOMEMORY                 = 7, // no enough memory
      MR_FAILED4_UNKNOWNUSER              = 8, // no user enrolled
      MR_FAILED4_MAXUSER                  = 9, // exceed maximum user number
      MR_FAILED4_FACEENROLLED             = 10,// this face has been enrolled
      MR_FAILED4_LIVENESSCHECK            = 12,// liveness check failed
      MR_FAILED4_TIMEOUT                  = 13,// exceed the time limit
      MR_FAILED4_AUTHORIZATION            = 14,// authorization failed
      MR_FAILED4_READ_FILE                = 19,// read file failed
      MR_FAILED4_WRITE_FILE               = 20,// write file failed
      MR_FAILED4_NO_ENCRYPT               = 21,// encrypt must be set
      MR_FAILED4_NO_RGBIMAGE              = 23 // rgb image is not ready
  };
  ```

* 人脸状态返回结果为int类型，对应定义如下：

  ```c
  // face state define
  typedef enum _EN_FACE_STATE {
      FACE_STATE_NORMAL                    = 0,   // normal state, the face is available
      FACE_STATE_NOFACE                    = 1,   // no face detected
      FACE_STATE_TOOUP                     = 2,   // face is too up side
      FACE_STATE_TOODOWN                   = 3,   // face is too down side
      FACE_STATE_TOOLEFT                   = 4,   // face is too left side
      FACE_STATE_TOORIGHT                  = 5,   // face is too right side
      FACE_STATE_TOOFAR                    = 6,   // face is too far
      FACE_STATE_TOOCLOSE                  = 7,   // face is too near
      FACE_STATE_EYEBROW_OCCLUSION         = 8,   // eyebrow occlusion
      FACE_STATE_EYE_OCCLUSION             = 9,   // eye occlusion
      FACE_STATE_FACE_OCCLUSION            = 10,  // face occlusion
      FACE_STATE_DIRECTION_ERROR           = 11,  // face direction error
      FACE_STATE_EYE_CLOSE_STATUS_OPEN_EYE = 12,  // open eye in the eye close status
      FACE_STATE_EYE_CLOSE_STATUS          = 13,  // confirm eye close status
      FACE_STATE_EYE_CLOSE_UNKNOW_STATUS   = 14   // eye close unknow status
  } EN_FACE_STATE;
  ```




##### 5.使用示例

* 接口文件为：libface_client.a及face_client.h，使用可参考同目录下face_client_test.c文件

  ```c
  void module_ready_cb(void)
  {
      printf("%s\n", __func__);
  }
  
  void verity_result_cb(int result, int usr_id)
  {
      printf("%s result:%d id:%d\n", __func__, result, usr_id);
  }
  
  void enroll_result_cb(int result, int usr_id)
  {
      printf("%s result:%d id:%d\n", __func__, result, usr_id);
  }
  
  void face_state_cb(int result)
  {
      printf("%s result:%d\n", __func__, result);
  }
  
  int main()
  {
      face_client_init();
  
      set_module_ready_cb(module_ready_cb);
      set_verify_result_cb(verity_result_cb);
      set_enroll_result_cb(enroll_result_cb);
      set_face_state_cb(face_state_cb);
  
      while (1) {
          int ret = test_node();
          if (ret)
              break;
          sleep(1);
      }
  
      face_client_deinit();
  }
  ```
