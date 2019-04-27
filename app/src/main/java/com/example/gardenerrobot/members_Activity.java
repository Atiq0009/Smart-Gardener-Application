package com.example.gardenerrobot;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class members_Activity extends AppCompatActivity {
    private Button backProfile;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_members_);
        backProfile =  findViewById(R.id.backprofile);
        backProfile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                backProf();
            }
        });
    }
    public void backProf(){
        Intent intent1 =new Intent(this, Home_Activity.class);
        startActivity(intent1);
        finish();
    }
}