// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_pet.proto

package com.cms.client.network.gpro;

public final class UserPet {
  private UserPet() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Pet_AdoptOne_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Pet_AdoptOne_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Pet_AdoptOne_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Pet_AdoptOne_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Pet_ReleaseOne_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Pet_ReleaseOne_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Pet_ReleaseOne_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Pet_ReleaseOne_ack_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\016user_pet.proto\022\003PRO\032\021db_internal.proto" +
      "\"#\n\020Pet_AdoptOne_req\022\017\n\007pet_iid\030\001 \001(\003\"P\n" +
      "\020Pet_AdoptOne_ack\022\016\n\006result\030\001 \001(\005\022$\n\003pet" +
      "\030\002 \001(\0132\022.PRO.DBUserPetItemH\000\210\001\001B\006\n\004_pet\"" +
      "\'\n\022Pet_ReleaseOne_req\022\021\n\tmypet_iid\030\001 \001(\003" +
      "\"7\n\022Pet_ReleaseOne_ack\022\021\n\tmypet_iid\030\001 \001(" +
      "\003\022\016\n\006result\030\002 \001(\005BT\n\033com.cms.client.netw" +
      "ork.gproP\001Z\025gamelib/protobuf/gpro\252\002\033com." +
      "cms.client.network.gprob\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
          com.cms.client.network.gpro.DbInternal.getDescriptor(),
        });
    internal_static_PRO_Pet_AdoptOne_req_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_PRO_Pet_AdoptOne_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Pet_AdoptOne_req_descriptor,
        new java.lang.String[] { "PetIid", });
    internal_static_PRO_Pet_AdoptOne_ack_descriptor =
      getDescriptor().getMessageTypes().get(1);
    internal_static_PRO_Pet_AdoptOne_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Pet_AdoptOne_ack_descriptor,
        new java.lang.String[] { "Result", "Pet", "Pet", });
    internal_static_PRO_Pet_ReleaseOne_req_descriptor =
      getDescriptor().getMessageTypes().get(2);
    internal_static_PRO_Pet_ReleaseOne_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Pet_ReleaseOne_req_descriptor,
        new java.lang.String[] { "MypetIid", });
    internal_static_PRO_Pet_ReleaseOne_ack_descriptor =
      getDescriptor().getMessageTypes().get(3);
    internal_static_PRO_Pet_ReleaseOne_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Pet_ReleaseOne_ack_descriptor,
        new java.lang.String[] { "MypetIid", "Result", });
    com.cms.client.network.gpro.DbInternal.getDescriptor();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
