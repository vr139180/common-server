using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.client.utils
{
    public class TimeUtil
    {
        public static long get_timestamp()
        {
            DateTime dt = new DateTime(1970, 1, 1);
            return (long)((DateTime.UtcNow - dt).TotalMilliseconds);
            
            /*
            DateTime windowsEpoch = new DateTime(1601, 1, 1, 0, 0, 0, 0);
            DateTime javaEpoch = new DateTime(1970, 1, 1, 0, 0, 0, 0);
            long epochDiff = (javaEpoch.ToFileTimeUtc() - windowsEpoch.ToFileTimeUtc()) / TimeSpan.TicksPerMillisecond;
            return ( DateTime.Now.ToFileTime() / TimeSpan.TicksPerMillisecond) - epochDiff;
            */
        }

        public static DateTime get_datetime_of_timestamp(long ts)
        {
            DateTime UTCBaseTime = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            DateTime dt = UTCBaseTime.Add(new TimeSpan( ts * TimeSpan.TicksPerMillisecond)).ToLocalTime();
            return dt;
        }  

        public static long get_unix_time()
        {
            DateTime dt = new DateTime(1970, 1, 1);
            return (long)((DateTime.UtcNow - dt).TotalSeconds);
        }
    }
}
