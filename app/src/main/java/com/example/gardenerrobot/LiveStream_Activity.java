package com.example.gardenerrobot;


import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class LiveStream_Activity extends AppCompatActivity {
    private Button mStartVideo, mEndVideo;
    private DatabaseReference db;
    private FirebaseAuth mAuth;
    private FirebaseAuth.AuthStateListener firebaseAuthListener;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_live_stream_);
        mStartVideo = (Button) findViewById(R.id.startStream);
        mEndVideo =  (Button) findViewById(R.id.endSteam);
        mAuth = FirebaseAuth.getInstance();
        firebaseAuthListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
                if (user != null){

                }else{
                    Intent intentUser = new Intent(LiveStream_Activity.this, MainActivity.class);
                    startActivity(intentUser);
                    finish();
                    return;
                }
            }
        };


        mStartVideo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                DatabaseReference current_user_id = FirebaseDatabase.getInstance().getReference().child("camera");

                int a = 1;

                current_user_id.setValue(a);
                Intent openApps = getPackageManager().getLaunchIntentForPackage("org.videolan.vlc");
                startActivity(openApps);
            }
        });

        mEndVideo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                endSteam();
            }
        });
    }



    public void endSteam(){
        Toast.makeText(LiveStream_Activity.this, "Video Streming Stopped", Toast.LENGTH_LONG).show();
        DatabaseReference current_user_id = FirebaseDatabase.getInstance().getReference().child("camera");

        int a = 0;

        current_user_id.setValue(a);

        Intent intent1 =new Intent(this, Home_Activity.class);
        startActivity(intent1);
        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        DatabaseReference current_user_id = FirebaseDatabase.getInstance().getReference().child("camera");
        int a = 0;
        current_user_id.setValue(a);
    }
}
