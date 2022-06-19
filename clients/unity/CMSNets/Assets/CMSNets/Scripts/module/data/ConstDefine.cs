using System;
using System.Collections.Generic;
using System.Text;

using com.cms.client.network.gpro;

namespace com.cms.module.data
{
    //Ƶ������
    public enum ChatChannelT
    {
        CCType_Channel = (int)ChatChannelType.ChatChannelTChannel,
        CCType_Union = (int)ChatChannelType.ChatChannelTUnion,
        //��������
        CCType_Single = (int)ChatChannelType.ChatChannelTSingle,
        //�Զ����������
        CCType_Custom = (int)ChatChannelType.ChatChannelTCustom,
    }

    public class ChatChannelInfoX
    {
        public ChatChannelT type;
        public Int64 channle_iid;

        public ChatChannelType to_channeltype()
        {
            switch( type)
            {
            case ChatChannelT.CCType_Channel:
                return ChatChannelType.ChatChannelTChannel;
            case ChatChannelT.CCType_Union:
                return ChatChannelType.ChatChannelTUnion;
            case ChatChannelT.CCType_Custom:
                return ChatChannelType.ChatChannelTCustom;
            default:
                return ChatChannelType.ChatChannelTSingle;
            }
        }
    }
}
