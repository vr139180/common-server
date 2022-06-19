using com.cms.module;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CMSContext : MonoBehaviour, ICMSListener
{
    // Start is called before the first frame update
    void Start()
    {
        CMSNetModule.instance().add_listener(this);

        Debug.Log("CMSContext add listener to CMSNetModule");
    }

    private void OnDestroy()
    {
        CMSNetModule.instance().del_listener(this);

        Debug.Log("CMSContext delete listener from CMSNetModule");
    }

    void Update()
    {
        CMSNetModule.instance().tick();
    }

    #region ICMSListener
    public void on_cms_netstate(CMSNetState state)
    {
    }

    public void on_cms_login()
    {
    }

    #endregion
}
