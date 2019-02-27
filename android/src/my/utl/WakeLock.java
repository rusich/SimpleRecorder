package my.utl;

import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtApplication;
import java.lang.String;
import android.app.Activity;
import android.os.PowerManager;
import android.content.Context;

public class WakeLock
{
    private Activity myActivity;
    protected PowerManager.WakeLock m_WakeLock = null;

    public WakeLock(Activity a)
    {
        myActivity = a;
    }

    public int configure()
    {
        System.out.println("Inside WakeLock::configure");

        try
        {
            final PowerManager pm = (PowerManager) myActivity.getSystemService(Context.POWER_SERVICE);

            m_WakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");
//             m_WakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "My Tag");
            // Does not work: https://stackoverflow.com/questions/5183859/partial-wake-lock-vs-screen-dim-wake-lock-in-download-thread
            m_WakeLock.acquire();

            return 42;
        }
        catch (Exception e)
        {
            System.out.println("WakeLock failed: " + e.toString());
        }

        return -1;
    }
}
