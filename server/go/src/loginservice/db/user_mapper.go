package db

import (
	"loginservice/db/entity"
	"loginservice/g"
)

func GetUserByAccount(acc string) (u *entity.UserAccount) {
	if len(acc) == 0 {
		return nil
	}
	db := g.DBS()

	u = &entity.UserAccount{Account: acc}
	has, err := db.Get(u)
	if err != nil {
		u = nil
	} else if !has {
		u = nil
	}

	return
}
