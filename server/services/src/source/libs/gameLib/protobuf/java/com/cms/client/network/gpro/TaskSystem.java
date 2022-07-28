// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: task_system.proto

package com.cms.client.network.gpro;

public final class TaskSystem {
  private TaskSystem() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_TaskRewardItem_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_TaskRewardItem_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_TaskRewardItems_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_TaskRewardItems_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_WaitList_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_WaitList_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_WaitList_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_WaitList_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_MyTaskList_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_MyTaskList_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_MyTaskList_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_MyTaskList_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_GetTask_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_GetTask_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_GetTask_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_GetTask_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_SubmitTask_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_SubmitTask_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_SubmitTask_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_SubmitTask_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_ObtainReward_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_ObtainReward_ntf_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_GiveupTask_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_GiveupTask_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Task_GiveupTask_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Task_GiveupTask_ack_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\021task_system.proto\022\003PRO\032\025global_interna" +
      "l.proto\032\021db_internal.proto\"T\n\016TaskReward" +
      "Item\022\"\n\004type\030\001 \001(\0162\024.PRO.TASKREWARD_TYPE" +
      "\022\020\n\010item_iid\030\002 \001(\005\022\014\n\004nums\030\003 \001(\005\"7\n\017Task" +
      "RewardItems\022$\n\007rewards\030\001 \003(\0132\023.PRO.TaskR" +
      "ewardItem\"C\n\021Task_WaitList_req\022#\n\006utoken" +
      "\030\001 \001(\0132\016.PRO.UserTokenH\000\210\001\001B\t\n\007_utoken\"V" +
      "\n\021Task_WaitList_ack\022#\n\006utoken\030\001 \001(\0132\016.PR" +
      "O.UserTokenH\000\210\001\001\022\021\n\ttask_iids\030\002 \003(\005B\t\n\007_" +
      "utoken\"E\n\023Task_MyTaskList_req\022#\n\006utoken\030" +
      "\001 \001(\0132\016.PRO.UserTokenH\000\210\001\001B\t\n\007_utoken\"\215\001" +
      "\n\023Task_MyTaskList_ack\022#\n\006utoken\030\001 \001(\0132\016." +
      "PRO.UserTokenH\000\210\001\001\022%\n\006groups\030\002 \001(\0132\025.PRO" +
      ".DBUserTaskGroups\022\037\n\005tasks\030\003 \001(\0132\020.PRO.D" +
      "BUserTasksB\t\n\007_utoken\"T\n\020Task_GetTask_re" +
      "q\022#\n\006utoken\030\001 \001(\0132\016.PRO.UserTokenH\000\210\001\001\022\020" +
      "\n\010task_iid\030\002 \001(\005B\t\n\007_utoken\"\225\001\n\020Task_Get" +
      "Task_ack\022#\n\006utoken\030\001 \001(\0132\016.PRO.UserToken" +
      "H\000\210\001\001\022\020\n\010task_iid\030\002 \001(\005\022\016\n\006result\030\003 \001(\005\022" +
      "&\n\004task\030\004 \001(\0132\023.PRO.DBUserTaskItemH\001\210\001\001B" +
      "\t\n\007_utokenB\007\n\005_task\"W\n\023Task_SubmitTask_r" +
      "eq\022#\n\006utoken\030\001 \001(\0132\016.PRO.UserTokenH\000\210\001\001\022" +
      "\020\n\010task_iid\030\002 \001(\005B\t\n\007_utoken\"g\n\023Task_Sub" +
      "mitTask_ack\022#\n\006utoken\030\001 \001(\0132\016.PRO.UserTo" +
      "kenH\000\210\001\001\022\020\n\010task_iid\030\002 \001(\005\022\016\n\006result\030\003 \001" +
      "(\005B\t\n\007_utoken\"\177\n\025Task_ObtainReward_ntf\022#" +
      "\n\006utoken\030\001 \001(\0132\016.PRO.UserTokenH\000\210\001\001\022\020\n\010t" +
      "ask_iid\030\002 \001(\005\022$\n\006reward\030\003 \001(\0132\024.PRO.Task" +
      "RewardItemsB\t\n\007_utoken\"W\n\023Task_GiveupTas" +
      "k_req\022#\n\006utoken\030\001 \001(\0132\016.PRO.UserTokenH\000\210" +
      "\001\001\022\020\n\010task_iid\030\002 \001(\005B\t\n\007_utoken\"\230\001\n\023Task" +
      "_GiveupTask_ack\022#\n\006utoken\030\001 \001(\0132\016.PRO.Us" +
      "erTokenH\000\210\001\001\022\020\n\010task_iid\030\002 \001(\005\022\016\n\006result" +
      "\030\003 \001(\005\022&\n\004task\030\004 \001(\0132\023.PRO.DBUserTaskIte" +
      "mH\001\210\001\001B\t\n\007_utokenB\007\n\005_task*\206\001\n\021TASK_TRIG" +
      "GER_TYPE\022\014\n\010TT_BEGIN\020\000\022\024\n\020TT_TRIGGER_TIM" +
      "ER\020\001\022\026\n\022TT_ROLEINFO_CHANGE\020\002\022\021\n\rTT_BAG_C" +
      "HANGE\020\004\022\026\n\022TT_BUILDING_CHANGE\020\010\022\n\n\006TT_MA" +
      "X\020\t*T\n\020TASK_GROUP_STATE\022\024\n\020TASKGROUP_ACC" +
      "EPT\020\000\022\024\n\020TASKGROUP_FINISH\020\001\022\024\n\020TASKGROUP" +
      "_GIVEUP\020\002*\257\001\n\nTASK_STATE\022\024\n\020TASKSTATE_AC" +
      "CEPT\020\000\022\033\n\016TASKSTATE_WAIT\020\377\377\377\377\377\377\377\377\377\001\022\026\n\022T" +
      "ASKSTATE_REACCEPT\020\001\022\024\n\020TASKSTATE_SUBMIT\020" +
      "\002\022\024\n\020TASKSTATE_FAILED\020\003\022\024\n\020TASKSTATE_GIV" +
      "EUP\020\004\022\024\n\020TASKSTATE_FINISH\020\005*H\n\017TASKREWAR" +
      "D_TYPE\022\027\n\023TASKREWARDT_BAGITEM\020\000\022\034\n\017TASKR" +
      "EWARDT_EXP\020\377\377\377\377\377\377\377\377\377\001BT\n\033com.cms.client." +
      "network.gproP\001Z\025gamelib/protobuf/gpro\252\002\033" +
      "com.cms.client.network.gprob\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
          com.cms.client.network.gpro.GlobalInternal.getDescriptor(),
          com.cms.client.network.gpro.DbInternal.getDescriptor(),
        });
    internal_static_PRO_TaskRewardItem_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_PRO_TaskRewardItem_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_TaskRewardItem_descriptor,
        new java.lang.String[] { "Type", "ItemIid", "Nums", });
    internal_static_PRO_TaskRewardItems_descriptor =
      getDescriptor().getMessageTypes().get(1);
    internal_static_PRO_TaskRewardItems_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_TaskRewardItems_descriptor,
        new java.lang.String[] { "Rewards", });
    internal_static_PRO_Task_WaitList_req_descriptor =
      getDescriptor().getMessageTypes().get(2);
    internal_static_PRO_Task_WaitList_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_WaitList_req_descriptor,
        new java.lang.String[] { "Utoken", "Utoken", });
    internal_static_PRO_Task_WaitList_ack_descriptor =
      getDescriptor().getMessageTypes().get(3);
    internal_static_PRO_Task_WaitList_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_WaitList_ack_descriptor,
        new java.lang.String[] { "Utoken", "TaskIids", "Utoken", });
    internal_static_PRO_Task_MyTaskList_req_descriptor =
      getDescriptor().getMessageTypes().get(4);
    internal_static_PRO_Task_MyTaskList_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_MyTaskList_req_descriptor,
        new java.lang.String[] { "Utoken", "Utoken", });
    internal_static_PRO_Task_MyTaskList_ack_descriptor =
      getDescriptor().getMessageTypes().get(5);
    internal_static_PRO_Task_MyTaskList_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_MyTaskList_ack_descriptor,
        new java.lang.String[] { "Utoken", "Groups", "Tasks", "Utoken", });
    internal_static_PRO_Task_GetTask_req_descriptor =
      getDescriptor().getMessageTypes().get(6);
    internal_static_PRO_Task_GetTask_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_GetTask_req_descriptor,
        new java.lang.String[] { "Utoken", "TaskIid", "Utoken", });
    internal_static_PRO_Task_GetTask_ack_descriptor =
      getDescriptor().getMessageTypes().get(7);
    internal_static_PRO_Task_GetTask_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_GetTask_ack_descriptor,
        new java.lang.String[] { "Utoken", "TaskIid", "Result", "Task", "Utoken", "Task", });
    internal_static_PRO_Task_SubmitTask_req_descriptor =
      getDescriptor().getMessageTypes().get(8);
    internal_static_PRO_Task_SubmitTask_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_SubmitTask_req_descriptor,
        new java.lang.String[] { "Utoken", "TaskIid", "Utoken", });
    internal_static_PRO_Task_SubmitTask_ack_descriptor =
      getDescriptor().getMessageTypes().get(9);
    internal_static_PRO_Task_SubmitTask_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_SubmitTask_ack_descriptor,
        new java.lang.String[] { "Utoken", "TaskIid", "Result", "Utoken", });
    internal_static_PRO_Task_ObtainReward_ntf_descriptor =
      getDescriptor().getMessageTypes().get(10);
    internal_static_PRO_Task_ObtainReward_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_ObtainReward_ntf_descriptor,
        new java.lang.String[] { "Utoken", "TaskIid", "Reward", "Utoken", });
    internal_static_PRO_Task_GiveupTask_req_descriptor =
      getDescriptor().getMessageTypes().get(11);
    internal_static_PRO_Task_GiveupTask_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_GiveupTask_req_descriptor,
        new java.lang.String[] { "Utoken", "TaskIid", "Utoken", });
    internal_static_PRO_Task_GiveupTask_ack_descriptor =
      getDescriptor().getMessageTypes().get(12);
    internal_static_PRO_Task_GiveupTask_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Task_GiveupTask_ack_descriptor,
        new java.lang.String[] { "Utoken", "TaskIid", "Result", "Task", "Utoken", "Task", });
    com.cms.client.network.gpro.GlobalInternal.getDescriptor();
    com.cms.client.network.gpro.DbInternal.getDescriptor();
  }

  // @@protoc_insertion_point(outer_class_scope)
}