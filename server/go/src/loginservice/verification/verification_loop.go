package verification

import (
	"cmslib/logx"
	"cmslib/server"
	"cmslib/utilc"
	"fmt"
	"gamelib/protobuf/gpro"
	"gamelib/redis"
	"loginservice/db"
	"loginservice/g"
	"loginservice/processor"
)

const VerifyLoopChanCaption = 1024

type verifyLoop struct {
	//ch recv event
	ch chan interface{}

	parent *VerifyControl
}

func newVerifyLoop(p *VerifyControl) (l *verifyLoop) {
	l = new(verifyLoop)
	l.parent = p
	l.ch = make(chan interface{}, VerifyLoopChanCaption)

	return
}

func (l *verifyLoop) run() {

	defer func() {
		l.parent.loopWG.Done()
	}()

	var err error

	for d := range l.ch {
		switch v := d.(type) {
		case error:
			err = v
		case *processor.LoginProcessor:
			l.userLogin(v)
		default:
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (l *verifyLoop) userLogin(p *processor.LoginProcessor) {
	if !p.IsValidProcessor() {
		logx.Debugf("......when user try to login, find that related netsession is closed. ignore this login request")
		return
	}

	if p.LoginType == 1 {
		l.dbLogin(p)
	} else if p.LoginType == 2 {
		l.tokenLogin(p)
	}
}

func (l *verifyLoop) dbLogin(p *processor.LoginProcessor) {
	user := db.GetUserByAccount(p.UserAccount)

	result := 0
	if user == nil {
		//用户名不存在
		result = 1
	}

	if result == 0 && user.Password != p.AccountPwd {
		//密码错误
		result = 2
	}

	utoken := ""
	if result == 0 {
		redisclient := g.GetRedis()

		ukey := fmt.Sprintf(redis.USERLOGIN_ONLINE_INFO, user.UserIid)
		ut := utilc.UuidGen()

		keys := []string{ukey, redis.USERLOGIN_ONLINE_TOKEN, redis.USERLOGIN_ONLINE_LASTUPDATE}

		//检查 用户缓存，是否在线
		dat, err := redisclient.Eval(REDIS_LOGIN_SC, keys, user.UserIid, ut, utilc.GetTimestamp())
		if err != nil {
			logx.Errorf("verifyLoop dbLogin redis script error:%s", err.Error())
			result = 5
		} else if dat[0] != "0" {
			result = 3
		} else {
			utoken = ut
		}
	}

	if result != 0 {
		ack := &gpro.User_LoginAck{}
		ack.Type = 1
		ack.Result = int32(result)

		p.SendMessage(ack)
		p.Close()
		return
	}

	p.Login(user.UserIid, utoken)

	logx.Debugf("user:%s iid:%d database login success", p.UserAccount, p.UserIid)

	g.RegistWaitProcess(p)
}

func (l *verifyLoop) tokenLogin(p *processor.LoginProcessor) {

	redisclient := g.GetRedis()
	ukey := fmt.Sprintf(redis.USERLOGIN_ONLINE_INFO, p.LoginUserId)

	ut := p.UserToken
	ret := 0

	u, e := redisclient.HGet(ukey, redis.USERLOGIN_ONLINE_TOKEN)
	if e != nil || u != ut {
		ret = 3 //toekn error
	}

	if ret == 0 {
		ut := utilc.UuidGen()

		keys := []string{ukey, redis.USERLOGIN_ONLINE_TOKEN, redis.USERLOGIN_ONLINE_LASTUPDATE}

		//检查 用户缓存，是否在线
		dat, err := redisclient.Eval(REDIS_LOGIN_SC, keys, p.LoginUserId, ut, utilc.GetTimestamp())
		if err != nil {
			logx.Errorf("verifyLoop tokenLogin redis script error:%s", err.Error())
			ret = 5
		} else if dat[0] != "0" {
			ret = 3
		} else {
			//用新的token取代旧 的
			p.UserToken = ut
		}
	}

	if ret != 0 {
		ack := &gpro.User_LoginAck{}
		ack.Type = 2
		ack.Result = int32(ret) //token error

		p.SendMessage(ack)
		p.Close()
		return
	}

	p.Login(p.LoginUserId, ut)

	logx.Debugf("user token:%s iid:%d redis login success", p.UserToken, p.LoginUserId)

	g.RegistWaitProcess(p)
}
