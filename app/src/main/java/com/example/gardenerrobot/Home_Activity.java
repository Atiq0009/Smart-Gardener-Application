package com.example.gardenerrobot;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.CardView;
import android.view.View;

import com.google.firebase.auth.FirebaseAuth;


public class Home_Activity extends AppCompatActivity {
    private CardView info;
    private CardView timer;
    private CardView livesteam;
    private CardView profile1;
    private CardView logout;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_);
        info = findViewById(R.id.information);
        info.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openInformation();
            }
        });
        timer = findViewById(R.id.Timeinput);
        timer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openTimer();
            }
        });
        livesteam =  findViewById(R.id.Livesteam);
        livesteam.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openLiveSteam();
            }
        });
        profile1 =  findViewById(R.id.profile);
        profile1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openProfile();
            }
        });
        logout =  findViewById(R.id.logout);
        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openLogout();
            }
        });
    }

    public void openInformation(){
        Intent intent1 =new Intent(this, Information.class);
        startActivity(intent1);
    }

    public void openTimer(){
        Intent intent2 =new Intent(this, Time_Activity.class);
        startActivity(intent2);
    }
    public void openLiveSteam(){
        Intent intent3 =new Intent(this, LiveStream_Activity.class);
        startActivity(intent3);
    }

    public void openProfile(){
        Intent intent4 =new Intent(this, members_Activity.class);
        startActivity(intent4);
    }
    public void openLogout(){
        FirebaseAuth.getInstance().signOut();
        Intent intent5 =new Intent(this, MainActivity.class);
        startActivity(intent5);
        finish();
    }
}
