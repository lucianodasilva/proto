package com.munchiecollision.protodatadriven;

import android.app.Notification;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.view.View.OnClickListener;

public class MainActivity extends ActionBarActivity {

    public native String [] ExecuteNativeTests ();

    public ActionBarActivity    _window;
    public Button               _cmd_button;
    public ListView             _result_view;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        _cmd_button = (Button)findViewById(R.id.cmd_button);
        _result_view = (ListView)findViewById(R.id.result_view);

        _window = this;

        _cmd_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                String [] results = ExecuteNativeTests();
                ArrayAdapter <String> adapter = new ArrayAdapter <String> (_window, R.layout.text_view_item, results);

                _result_view.setAdapter(adapter);
            }
        });


    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
