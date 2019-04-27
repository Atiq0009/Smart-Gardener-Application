package com.example.gardenerrobot;

import android.content.Intent;
import android.media.MediaTimestamp;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.security.PublicKey;
import java.sql.Time;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Time_Activity extends AppCompatActivity {
    private EditText mDCMorning, mDCNoon, mDCAfternoon;
    private EditText mSecFirst, mSecSecond;
    private Button saveTimeDB, backTimeDB;

    private FirebaseAuth mAuth;
    private FirebaseAuth.AuthStateListener firebaseAuthListener;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_time_);
        mAuth = FirebaseAuth.getInstance();
        firebaseAuthListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
                if (user != null){

                }else{
                    Intent intentUser = new Intent(Time_Activity.this, MainActivity.class);
                    startActivity(intentUser);
                    finish();
                    return;
                }
            }
        };

        mDCMorning = (EditText) findViewById(R.id.timeDCMorning);
        mDCNoon = (EditText) findViewById(R.id.timeDCNoon);
        mDCAfternoon = (EditText) findViewById(R.id.timeDCAfternoon);
        mSecFirst = (EditText) findViewById(R.id.timeSecFirst);
        mSecSecond = (EditText) findViewById(R.id.timeSecSecond);






        saveTimeDB = (Button) findViewById(R.id.saveTime);
        backTimeDB = (Button) findViewById(R.id.BackTime);

        saveTimeDB.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openTimesave();
            }
        });

        backTimeDB.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                openHomeBack();
            }
        });

    }

    @Override
    protected void onStart() {

        super.onStart();
        mAuth.addAuthStateListener(firebaseAuthListener);
        DatabaseReference mRef = FirebaseDatabase.getInstance().getReference().child("TimeTable").child("DCMorning");
        mRef.addChildEventListener(new ChildEventListener() {
            @Override
            public void onChildAdded(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {

            }
            @Override
            public void onChildChanged(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {
                String sa = dataSnapshot.getValue().toString();
                Toast.makeText(Time_Activity.this,"Changed",Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onChildRemoved(@NonNull DataSnapshot dataSnapshot) {


            }

            @Override
            public void onChildMoved(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {

            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                /*Toast.makeText(Time_Activity.this, "Database error", Toast.LENGTH_SHORT).show();*/
            }
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        mAuth.removeAuthStateListener(firebaseAuthListener);
    }

    public void openTimesave(){

        String dcMorning = mDCMorning.getText().toString();
        String dcNoon = mDCNoon.getText().toString();
        String dcAfternoon = mDCAfternoon.getText().toString();
        String sceFirst = mSecFirst.getText().toString();
        String sceSecond = mSecSecond.getText().toString();

        DatabaseReference mRef = FirebaseDatabase.getInstance().getReference().child("TimeTable");

        Map newTime = new HashMap();
        newTime.put("DCMorning", dcMorning);
        newTime.put("DCNoon", dcNoon);
        newTime.put("DCAfternoon", dcAfternoon);
        newTime.put("SecFirst", sceFirst);
        newTime.put("SecSecond", sceSecond);

        mRef.setValue(newTime);

        Intent intent1 =new Intent(this, Home_Activity.class);
        startActivity(intent1);

    }

    public void openHomeBack(){
        Intent intent1 =new Intent(this, Home_Activity.class);
        startActivity(intent1);
        finish();
    }
 private void showData(DataSnapshot dataSnapshot){
        for(DataSnapshot ds: dataSnapshot.getChildren()){
            timeStore ts =new timeStore();
            ts.setDCAfternoon(ds.child("TimeTable").getValue(timeStore.class).getDCAfternoon());
            ts.setDCNoon(ds.child("TimeTable").getValue(timeStore.class).getDCNoon());
            ts.setDCMorning(ds.child("TimeTable").getValue(timeStore.class).getDCMorning());
            ts.setSecFirst(ds.child("TimeTable").getValue(timeStore.class).getSecFirst());
            ts.setSecSecond(ds.child("TimeTable").getValue(timeStore.class).getSecSecond());

            mDCAfternoon.setText(ts.getDCAfternoon());

        }



    }




}
