package entity

import "time"

type UserTaskGroup struct {
	Ver_ uint32 `xorm:"'ver_'"`

	Iid        int64     `xorm:"pk 'iid'"`
	RoleIid    int64     `xorm:"'role_iid'"`
	TaskGroup  int32     `xorm:"'task_group'"`
	GroupCell  int32     `xorm:"'group_cell'"`
	CellData   string    `xorm:"'cell_data'"`
	GState     int32     `xorm:"'gstate'"`
	TriggLevel int32     `xorm:"'trigg_level'"`
	CreateTime time.Time `xorm:"'createtime'"`
	EndTime    time.Time `xorm:"'endtime'"`
}

func (u UserTaskGroup) TableName() string {
	return "user_taskgroup"
}

type UserTaskInfo struct {
	Ver_ uint32 `xorm:"'ver_'"`

	Iid             int64     `xorm:"pk 'iid'"`
	RoleIid         int64     `xorm:"'role_iid'"`
	TaskIid         int32     `xorm:"'task_iid'"`
	MyTaskGroup     int64     `xorm:"'my_taskgroup'"`
	TaskGroup       int32     `xorm:"'task_group'"`
	QState          int32     `xorm:"'qstate'"`
	AcceptLevel     int32     `xorm:"'accept_level'"`
	CycleTask       int32     `xorm:"'cycle_task'"`
	CycleNum        int32     `xorm:"'cycle_num'"`
	CreateTime      time.Time `xorm:"'createtime'"`
	FirstUpdateTime time.Time `xorm:"'firstupdatetime'"`
	LastUpdateTime  time.Time `xorm:"'lastupdatetime'"`
	TaskDatas       string    `xorm:"'task_datas'"`
	SourceIid       string    `xorm:"'source_iid'"`
}

func (u UserTaskInfo) TableName() string {
	return "user_taskinfo"
}
