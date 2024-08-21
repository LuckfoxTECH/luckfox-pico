### RV1106 RMSL人脸模块接口说明

* 接口函数分为两类，1.ap向模组发起请求，2.模组通知ap消息

##### 1. AP请求接口

* 请求人脸解锁

  ```c
  /*******************
  输入参数: seconds: 解锁的超时时间
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int intf_set_module_face_verify(int seconds)
  ```

* 请求人脸注册

  ```c
  /*******************
  输入参数:  seconds: 注册的超时时间
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  int intf_set_module_face_enroll(int seconds)
  ```

* 请求清除人脸

  ```c
  /*******************
  输入参数:  无
  返回参数： 0：请求成功
           非0：请求失败
  *********************/
  void intf_module_clr_face_data(void)
  ```


##### 2. 模组消息通知API


此类api为模组主动发起，用户可在函数内加自定义处理。

* 人脸模组准备就绪，通知ap模组已就绪，模组主动调用

  ```c
  /*******************
  输入参数:  无
  返回参数： 无
  *********************/
  void intf_module_ready()
  ```

* 人脸解锁结果消息，模组处理完解锁任务主动调用

  ```c
  /*******************
  输入参数: result：解锁结果
			usr_id：解锁得到的用户ID
  返回参数： 无
  *********************/
  void intf_module_send_verify_result(int result, int usr_id)
  ```

* 人脸注册结果消息，模组处理完注册任务主动调用

  ```c
  /*******************
  输入参数: result：注册结果
			usr_id：注册得到的用户ID
  返回参数： 无
  *********************/
  void intf_module_send_enroll_result(int result, int usr_id)
  ```

##### 3.注册及解锁结果定义

* 返回结果为int类型，对应的定义如下

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
