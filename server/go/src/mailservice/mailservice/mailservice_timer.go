package mailservice

func (l *MailService) RegisterTimer() {
	l.softTimer.AddTimer(3*1000, l.OnSystemMailLoadTimer)
}

func (l *MailService) OnSystemMailLoadTimer(int64, int, int64) (finish bool) {
	finish = true

	l.mailboxCtrl.LoadSystemMailBox()

	return
}
