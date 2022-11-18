package com.cms.commons.times;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 *
 * 高并发场景下System.currentTimeMillis()的性能问题的优化
 */
public class SystemClock
{
	private static final String THREAD_NAME = "system.clock";
	private static final SystemClock MILLIS_CLOCK = new SystemClock(1);
	private final long precision;
	private final AtomicLong now;

	private SystemClock(long precision)
	{
		this.precision = precision;
		now = new AtomicLong(System.currentTimeMillis());
		scheduleClockUpdating();
	}

	public static SystemClock millisClock()
	{
		return MILLIS_CLOCK;
	}

	private void scheduleClockUpdating()
	{
		ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor(runnable -> {
			Thread thread = new Thread(runnable, THREAD_NAME);
			thread.setDaemon(true);
			return thread;
		});

		scheduler.scheduleAtFixedRate(() -> now.set(System.currentTimeMillis()), precision, precision, TimeUnit.MILLISECONDS);
	}

	public long now()
	{
		return now.get();
	}
}
