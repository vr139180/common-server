using com.cms.module;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CMSLogin : MonoBehaviour, ICMSListener
{
    public string ip = "127.0.0.1";
    public int port = 9001;

    public InputField user_account;
    public InputField user_pwd;

    public Text result_txt;

    // Start is called before the first frame update
    void Start()
    {
        CMSNetModule.instance().set_loginserver(this.ip, this.port);
        CMSNetModule.instance().add_listener(this);

        Debug.Log("CMSLogin add listener to CMSNetModule");
    }

    // Update is called once per frame
    void Update()
    {
        CMSNetModule.instance().tick();
    }

    private void OnDestroy()
    {
        CMSNetModule.instance().del_listener(this);

        Debug.Log("CMSLogin delete listener from CMSNetModule");
    }

    public void OnLoginClick()
    {
        Debug.Log("----clicked-----------");
        Debug.LogFormat("usr:{0} pwd:{1}", user_account.text, user_pwd.text);

        CMSNetModule.instance().login_db(user_account.text, user_pwd.text);
    }

    #region ICMSListener
    public void on_cms_netstate(CMSNetState state)
    {
        this.result_txt.text = string.Format("netstate:{0}", state);
    }

    public void on_cms_login()
    {
        this.result_txt.text = "login success";
    }

    #endregion
}
