using System;
using System.Collections.Generic;
using System.Text;

using com.cms.client.network.gpro;

namespace com.cms.module.data
{
    public class UserData
    {
        public UserData()
        {
        }

        private DBUserHome           homeInfo;
        //建筑结构信息
        private DBUserHomeStructure  homeStructs;
        //宠物信息
        private DBUserPets userPets;

        //roles
        private DBUserRoles rolesOfUser;
    }
}
