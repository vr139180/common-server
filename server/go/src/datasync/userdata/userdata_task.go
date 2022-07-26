package userdata

import (
	"cmslib/redisutil"
	"cmslib/utilc"
	"datasync/db/entity"
	"gamelib/protobuf/gpro"
	"gamelib/redis"

	"xorm.io/xorm"
)

type userTasks struct {
	*userOperator

	taskgroup_data     gpro.DBUserTaskGroups
	taskgroup_end_data gpro.DBUserTaskGroupEnds

	task_data     gpro.DBUserTasks
	task_end_data gpro.DBUserTaskEnds

	b_modify bool
}

func (u *userTasks) LoadFromRedis(mdata map[string]string, r *redisutil.RedisUtil) bool {
	u.b_modify = false

	if !u.stringToMessage(mdata, redis.USER_DETAIL_TASKGROUPS, &u.taskgroup_data) {
		return false
	}
	if !u.stringToMessageIgnore(mdata, redis.USER_DETAIL_TASKGROUPS_END, &u.taskgroup_end_data) {
		return false
	}

	if !u.stringToMessage(mdata, redis.USER_DETAIL_TASKS, &u.task_data) {
		return false
	}
	if !u.stringToMessageIgnore(mdata, redis.USER_DETAIL_TASKS_END, &u.task_end_data) {
		return false
	}

	return true
}

func (u *userTasks) BuildValidData(dbver uint32, cachever uint32) {

	//--------------------------------------------------------------
	tgs := make([]*gpro.DBUserTaskGroup, 0)
	for _, d1 := range u.taskgroup_data.Groups {
		if d1.GetVer_() > dbver {
			u.b_modify = true
			tgs = append(tgs, d1)
		}
	}
	u.taskgroup_data.Groups = tgs

	tges := make([]*gpro.DBUserTaskGroupEnd, 0)
	for _, d1 := range u.taskgroup_end_data.Groups {
		if d1.GetVer_() > dbver {
			u.b_modify = true
			tges = append(tges, d1)
		}
	}
	u.taskgroup_end_data.Groups = tges

	//----------------------------------------------
	ts := make([]*gpro.DBUserTaskItem, 0)
	for _, d1 := range u.task_data.Items {
		if d1.GetVer_() > dbver {
			u.b_modify = true
			ts = append(ts, d1)
		}
	}
	u.task_data.Items = ts

	tes := make([]*gpro.DBUserTaskEndItem, 0)
	for _, d1 := range u.task_end_data.Items {
		if d1.GetVer_() > dbver {
			u.b_modify = true
			tes = append(tes, d1)
		}
	}
	u.task_end_data.Items = tes
}

func (u *userTasks) SaveDatabase(session *xorm.Session) error {
	if !u.b_modify {
		return nil
	}

	//task group
	for _, d1 := range u.taskgroup_data.Groups {
		bean := u.redisTGBean2DbBean(d1)
		idbean := &entity.UserTaskGroup{Iid: d1.GetIid()}

		err := u.InsertOrUpdate(session, idbean, bean)
		if err != nil {
			return err
		}
	}

	for _, d1 := range u.taskgroup_end_data.Groups {
		bean := u.redisTGEBean2DbBean(d1)

		_, err := session.Update(bean)
		if err != nil {
			return err
		}
	}

	//tasks
	for _, d1 := range u.task_data.Items {
		bean := u.redisTBean2DbBean(d1)
		idbean := &entity.UserTaskInfo{Iid: d1.GetIid()}

		err := u.InsertOrUpdate(session, idbean, bean)
		if err != nil {
			return err
		}
	}

	for _, d1 := range u.task_end_data.Items {
		bean := u.redisTEBean2DbBean(d1)

		_, err := session.Update(bean)
		if err != nil {
			return err
		}
	}

	return nil
}

func (u *userTasks) redisTGBean2DbBean(dat *gpro.DBUserTaskGroup) (b *entity.UserTaskGroup) {
	b = &entity.UserTaskGroup{}
	b.Ver_ = dat.GetVer_()

	b.Iid = dat.GetIid()
	b.RoleIid = dat.GetRoleIid()
	b.TaskGroup = dat.GetTaskGroup()
	b.GroupCell = dat.GetGroupCell()

	attrdat := dat.GetCellData()
	if attrdat == nil {
		b.CellData = ""
	} else {
		b.CellData = utilc.ProtoToString(attrdat)
	}

	b.GState = dat.GetGstate()
	b.TriggLevel = dat.GetTriggLevel()
	b.CreateTime = utilc.SecondToTime(dat.GetCreatetime())
	b.EndTime = utilc.SecondToTime(dat.GetEndtime())

	return b
}

func (u *userTasks) redisTGEBean2DbBean(dat *gpro.DBUserTaskGroupEnd) (b *entity.UserTaskGroup) {
	b = &entity.UserTaskGroup{}
	b.Ver_ = dat.GetVer_()

	b.Iid = dat.GetIid()
	b.TaskGroup = dat.GetTaskGroup()
	b.GState = dat.GetGstate()
	b.EndTime = utilc.SecondToTime(dat.GetEndtime())

	return b
}

func (u *userTasks) redisTBean2DbBean(dat *gpro.DBUserTaskItem) (b *entity.UserTaskInfo) {
	b = &entity.UserTaskInfo{}
	b.Ver_ = dat.GetVer_()

	b.Iid = dat.GetIid()
	b.RoleIid = dat.GetRoleIid()
	b.TaskIid = dat.GetTaskIid()
	b.MyTaskGroup = dat.GetMyTaskgroup()
	b.TaskGroup = dat.GetTaskGroup()
	b.QState = dat.GetQstate()
	b.AcceptLevel = dat.GetAcceptLevel()
	if dat.GetCycleTask() {
		b.CycleTask = 1
	} else {
		b.CycleTask = 0
	}
	b.CycleNum = dat.GetCycleNum()
	b.CreateTime = utilc.SecondToTime(dat.GetCreatetime())
	b.FirstUpdateTime = utilc.SecondToTime(dat.GetFirstupdatetime())
	b.LastUpdateTime = utilc.SecondToTime(dat.GetLastupdatetime())
	attrdat := dat.GetTaskDatas()
	if attrdat == nil {
		b.TaskDatas = ""
	} else {
		b.TaskDatas = utilc.ProtoToString(attrdat)
	}

	b.SourceIid = dat.GetSourceIid()

	return b
}

func (u *userTasks) redisTEBean2DbBean(dat *gpro.DBUserTaskEndItem) (b *entity.UserTaskInfo) {
	b = &entity.UserTaskInfo{}
	b.Ver_ = dat.GetVer_()

	b.Iid = dat.GetIid()
	b.TaskIid = dat.GetTaskIid()
	b.QState = dat.GetQstate()
	b.LastUpdateTime = utilc.SecondToTime(dat.GetLastupdatetime())

	return b
}
