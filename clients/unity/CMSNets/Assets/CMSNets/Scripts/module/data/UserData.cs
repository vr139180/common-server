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
        //�����ṹ��Ϣ
        private DBUserHomeStructure  homeStructs;
        //������Ϣ
        private DBUserPets userPets;

        //roles
        private DBUserRoles rolesOfUser;
    }
}
