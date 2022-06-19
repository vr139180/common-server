/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     6/1/2022 12:01:34 PM                         */
/*==============================================================*/


drop procedure if exists get_role_data;

drop table if exists pet_system;

drop table if exists role_baseinfo;

drop table if exists sys_mail;

drop table if exists user_account;

drop table if exists user_bag_item;

drop table if exists user_home;

drop table if exists user_home_structure;

drop table if exists user_mail;

drop table if exists user_mailbox;

drop table if exists user_pets;

drop table if exists user_unbind_item;

/*==============================================================*/
/* Table: pet_system                                            */
/*==============================================================*/
create table pet_system
(
   pet_iid              bigint not null,
   pet_type             int not null default 1 comment '1:dog',
   pet_resid            bigint not null,
   pet_sex              smallint not null default 1 comment '1:公 2:母 3:无性别',
   pet_name             varchar(32) not null,
   primary key (pet_iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on pet_system
(
   pet_iid
);

/*==============================================================*/
/* Table: role_baseinfo                                         */
/*==============================================================*/
create table role_baseinfo
(
   role_iid             bigint not null,
   user_iid             bigint not null comment '角色所属用户',
   nickname             varchar(32) not null,
   registime            datetime not null,
   ver_                 int unsigned not null default 1,
   primary key (role_iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create index index_1 on role_baseinfo
(
   user_iid
);

/*==============================================================*/
/* Table: sys_mail                                              */
/*==============================================================*/
create table sys_mail
(
   iid                  bigint not null auto_increment,
   title                varchar(128) not null comment '标题',
   contents             varchar(512) not null comment '内容',
   attachment           smallint not null default 0 comment '0:无附件 1:有附件',
   attachinfo           varchar(512) default '' comment '附件信息json字符串',
   createtime           timestamp not null default current_timestamp comment '创建时间',
   primary key (iid)
)
type = innodb;

alter table sys_mail comment '存放系统邮件
例如各类补偿邮件，用户需要主动的获取';

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on sys_mail
(
   iid
);

/*==============================================================*/
/* Table: user_account                                          */
/*==============================================================*/
create table user_account
(
   user_iid             bigint not null,
   account              national varchar(64) not null,
   pwd                  national varchar(64) not null,
   state                smallint not null default 0 comment '0:normal 1:禁用 2::删除',
   primary key (user_iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_account
(
   account
);

/*==============================================================*/
/* Table: user_bag_item                                         */
/*==============================================================*/
create table user_bag_item
(
   iid                  bigint not null,
   role_iid             bigint not null,
   ver_                 int unsigned not null default 1,
   itype                int not null default 0 comment '物品类型',
   item_resiid          int not null default 0 comment '物品编号',
   item_num             int not null default 0 comment '物品数量',
   slot_num             int not null default 1 comment '占用格子数',
   primary key (iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_bag_item
(
   iid
);

/*==============================================================*/
/* Index: index_2                                               */
/*==============================================================*/
create index index_2 on user_bag_item
(
   role_iid
);

/*==============================================================*/
/* Table: user_home                                             */
/*==============================================================*/
create table user_home
(
   role_iid             bigint not null comment '和user_base同一个id',
   home_name            varchar(64) not null,
   ground_resid         bigint,
   look_at              varchar(64),
   geo_pos              varchar(32),
   reside_time          int default 0,
   last_residedate      datetime,
   ver_                 int unsigned not null default 1,
   primary key (role_iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_home
(
   role_iid
);

/*==============================================================*/
/* Table: user_home_structure                                   */
/*==============================================================*/
create table user_home_structure
(
   building_iid         bigint not null,
   home_iid             bigint not null comment '和user_iid同一个值',
   parent_building      bigint not null default 0 comment '=0 表示附着在home上',
   building_resid       bigint,
   look_at              varchar(64),
   building_pos         varchar(32),
   ver_                 int unsigned not null default 1,
   primary key (building_iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_home_structure
(
   building_iid
);

/*==============================================================*/
/* Table: user_mail                                             */
/*==============================================================*/
create table user_mail
(
   iid                  bigint not null auto_increment,
   stype                int not null comment '0系统邮件 1工会邮件',
   sender_iid           bigint not null default 0 comment '发送人 stype=1 工会id',
   receiver_iid         bigint not null comment '接收人',
   attachment           smallint not null default 0 comment '0:无附件 1:有附件 2:已使用附件',
   attachinfo           varchar(512) default '' comment '附件信息json字符串',
   title                varchar(128) not null comment '标题',
   contents             varchar(512) not null comment '内容',
   createtime           timestamp not null default current_timestamp comment '创建时间',
   readtime             timestamp comment '邮件读取时间,linux时间',
   primary key (iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_mail
(
   iid
);

/*==============================================================*/
/* Index: index_2                                               */
/*==============================================================*/
create index index_2 on user_mail
(
   receiver_iid
);

/*==============================================================*/
/* Index: index_3                                               */
/*==============================================================*/
create index index_3 on user_mail
(
   stype,
   sender_iid
);

/*==============================================================*/
/* Table: user_mailbox                                          */
/*==============================================================*/
create table user_mailbox
(
   role_iid             bigint not null comment 'role_iid一致',
   ver_                 int unsigned not null default 1,
   sysmail_sync         bigint not null default 0 comment '最后同步的系统邮件id',
   lastlist_mail        bigint not null default 0 comment '最后获取的邮件iid，用来计算新邮件通知',
   offline_mail         bigint not null default 0 comment '最新的一个离线邮件id，如果用户未在线，通过这个计算新邮件',
   unread_num           int not null default 0 comment '未读邮件数',
   totle_num            int not null default 0 comment '总的邮件数',
   primary key (role_iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_mailbox
(
   role_iid
);

/*==============================================================*/
/* Table: user_pets                                             */
/*==============================================================*/
create table user_pets
(
   mypet_iid            bigint not null,
   role_iid             bigint not null,
   pet_iid              bigint not null default 1,
   pet_age              int not null default 0 comment '到月份',
   birthday             datetime not null,
   ver_                 int unsigned not null default 1,
   primary key (mypet_iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_pets
(
   mypet_iid
);

/*==============================================================*/
/* Table: user_unbind_item                                      */
/*==============================================================*/
create table user_unbind_item
(
   iid                  bigint not null,
   role_iid             bigint not null,
   itype                int not null default 0 comment '物品类型',
   item_resiid          int not null default 0 comment '物品编号',
   item_num             int not null default 0 comment '物品数量',
   source_type          int not null default 0 comment '来源类型,系统邮件,支付购买',
   source_iid           varchar(33) not null default '' comment '批号，同一批的uuid',
   createtime           timestamp not null comment '创建时间',
   primary key (iid)
)
type = innodb;

/*==============================================================*/
/* Index: index_1                                               */
/*==============================================================*/
create unique index index_1 on user_unbind_item
(
   iid
);

/*==============================================================*/
/* Index: index_2                                               */
/*==============================================================*/
create index index_2 on user_unbind_item
(
   role_iid
);

/*==============================================================*/
/* Index: index_3                                               */
/*==============================================================*/
create index index_3 on user_unbind_item
(
   source_type,
   source_iid
);


delimiter //

create procedure get_role_data(in uid bigint)
begin 
    
    select ver_,role_iid,user_iid,nickname,from_unixtime(registime) from role_baseinfo where role_iid=uid;
    select ver_,role_iid,home_name,ground_resid,look_at,geo_pos,reside_time,from_unixtime(last_residedate) from user_home where role_iid=uid;
    select ver_,building_iid,home_iid,parent_building,building_resid,look_at,building_pos from user_home_structure where home_iid=uid;
    select ver_,mypet_iid,role_iid,pet_iid,pet_age,from_unixtime(birthday) from user_pets where role_iid=uid;
end
//

delimiter ;

