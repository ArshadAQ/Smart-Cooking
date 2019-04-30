package com.example.stoveapp.AccountActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import com.example.stoveapp.R;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import android.os.CountDownTimer;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Locale;


public class timeractivity extends AppCompatActivity {



     long START_TIME_IN_MILLIS;

    private TextView mTextViewCountDown;
    private Button mButtonStartPause;
    private Button mButtonReset;
    String actmod,burnerinuse,food;
    long timeinfire,r1,r2,r3,r4,r5;
    long minutes,seconds;
    int i=0,getextra;
    Button stopcook,cookmore,finishcook;

    long duration_to_run,getextra1;

    boolean stop = false;

    long m2 = 0;
    boolean firstListen1 =false;
    long prevSum = 0;
    TextView knob,burshow;



    private CountDownTimer mCountDownTimer;

    private boolean mTimerRunning;
    TextView mQnATimer;
    DatabaseReference mTimerRef,mTimerRef1,mTimerRef2,mTimerRef3,mTimerRef4,mTimerRef6,mTimerRef7, temp, temp1;

     long mTimeLeftInMillis1,m1;
     long ff;
     Intent fl,fl2;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_timeractivity);


         assignvalues();

         stopcook=(Button) findViewById(R.id.stopcook);
        cookmore=(Button) findViewById(R.id.cookmore);
         finishcook=(Button) findViewById(R.id.finishcook);
        knob=findViewById(R.id.knobst);
        burshow=findViewById(R.id.burst);
        burshow.setText("Burner In Use :"+burnerinuse);



        stopcook.setVisibility(View.VISIBLE);
        cookmore.setVisibility(View.GONE);
        finishcook.setVisibility(View.GONE);
        final Intent finishintent=new Intent(timeractivity.this,MainActivity.class);









        /*FirebaseDatabase database = FirebaseDatabase.getInstance();
        Intent intent1 = getIntent();

        DatabaseReference ref = FirebaseDatabase.getInstance().getReference();
        FirebaseUser user2 = FirebaseAuth.getInstance().getCurrentUser();
        DatabaseReference mostafa = ref.child("Model_no").child(user2.getUid());*/
        mQnATimer = (TextView) findViewById(R.id.text_view_countdown);
        //final Intent fl=getIntent();

        //ff=(long)(fl.getFloatExtra("time",0))*60000;
        //mTimeLeftInMillis=ff;

        mTimerRef = FirebaseDatabase.getInstance().getReference(actmod).child(burnerinuse).child("req_to_start_stop");
        mTimerRef3= FirebaseDatabase.getInstance().getReference(actmod).child(burnerinuse).child("retrain");
        mTimerRef4= FirebaseDatabase.getInstance().getReference(actmod).child(burnerinuse).child("continue");
        mTimerRef6= FirebaseDatabase.getInstance().getReference(actmod).child(burnerinuse).child("knob_status");
       // mTimerRef7=FirebaseDatabase.getInstance().getReference(actmod).child(burnerinuse).child("extra_time");

//        mTimerRef7.addValueEventListener(new ValueEventListener() {
//            @Override
//            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
//                getextra=dataSnapshot.getValue(Integer.class);
//
//
//            }
//
//            @Override
//            public void onCancelled(@NonNull DatabaseError databaseError) {
//
//            }
//        });


        mTimerRef1=FirebaseDatabase.getInstance().getReference(actmod).child("Food_profile").child(food).child("steps");

//        boolean firstListen = false;
//        firstListen1 = false;
        mTimerRef2=FirebaseDatabase.getInstance().getReference(actmod).child("Food_profile").child(food).child("steps");
        mTimerRef2.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                boolean firstListen = true;
                firstListen1 = firstListen;
                r4 = 0;
                for(DataSnapshot snapshot: dataSnapshot.getChildren()){
//                    temp = snapshot.getRef();
//                    temp1 = temp.orderByKey().limitToFirst(1).getRef();
//                     = snapshot.getChildren().getClass();
                    for(DataSnapshot snap : snapshot.getChildren()){
                        r4 += (long)(snap.getValue(Float.class)*600000);
                    }
                }
//                r1=(long)(dataSnapshot.child("mode1").child("ON").getValue(Float.class)*60000);
//                r2=(long)(dataSnapshot.child("mode2").child("ON-OFF").getValue(Float.class)*60000);
//                r3=(long)(dataSnapshot.child("mode3").child("ON-SIM").getValue(Float.class)*60000);
//                r5=(long)(dataSnapshot.child("mode4").child("SIM").getValue(Float.class)*60000);


//                r4=r1+r2+r3+r5;

//                m2 = r4;
//                prevSum = r4;
                Log.d("ab1","thy"+r4);
//                counttime(r4);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });


      /*  if(getextra!=0)
        {
            getextra1=((long)getextra);
            counttime(getextra1);

        }*/


            mTimerRef1.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    if (firstListen1 == false) {
                        return;
                    }
                    r4 = 0;
                    for(DataSnapshot snapshot: dataSnapshot.getChildren()){
//                    temp = snapshot.getRef();
//                    temp1 = temp.orderByKey().limitToFirst(1).getRef();
//                     = snapshot.getChildren().getClass();
                        for(DataSnapshot snap : snapshot.getChildren()){
                            r4 += (long)(snap.getValue(Float.class)*60000);
                        }
                    }
//                    r1 = (long) (dataSnapshot.child("mode1").child("ON").getValue(Float.class) * 60000);
//                    r2 = (long) (dataSnapshot.child("mode2").child("ON-OFF").getValue(Float.class) * 60000);
//                    r3 = (long) (dataSnapshot.child("mode3").child("ON-SIM").getValue(Float.class) * 60000);
//                    r5 = (long) (dataSnapshot.child("mode4").child("SIM").getValue(Float.class) * 60000);
//
//                    r4 = r1 + r2 + r3 + r5;
//                prevSum = r4;
//                long diff = prevSum - r4;
                    long diff = r4 - prevSum;
                    Log.d("aa1", "thy" + m2);
                    Log.d("aa2", "thy" + r4);
                    Log.d("aa3", "thy" + prevSum);
//                Log.d("aa1","thy"+diff)


                    if (m2 == 0) {
                        counttime(diff - m2);
                    } else {
                        counttime(m2 + diff);
                    }

//                    counttime(m2 - diff);
//                counttime(diff-m2);s
//                counttime(r4);


//                m2 -= diff;
                    prevSum = r4;

                    Log.d("aa", "thy" + m2);
                    Log.d("aa2", "thy" + diff);
                    // Log.d("aa3"
//                if(i==1)
//                {  Log.d("thlat","io"+r4);
//                    mTimerRef.setValue(r4);
//                }


                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });

            mTimerRef6.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    knob.setText("Knob Status : " + dataSnapshot.getValue().toString());


                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });



       /* public void counttime() {

            mCountDownTimer = new CountDownTimer(mTimeLeftInMillis, 1000) {
                @Override
                public void onTick(long millisUntilFinished) {
                    mTimeLeftInMillis = millisUntilFinished;
                    updateCountDownText();
                }

                @Override
                public void onFinish() {
                    mTimerRunning = false;
                    stopcook.setVisibility(View.GONE);
                    cookmore.setVisibility(View.VISIBLE);
                    finishcook.setVisibility(View.VISIBLE);
                    //when timer will finish
                    //submit marks to firebase and close this activity
                    //once marks is been submitted to firebase then close the quiz
                    Toast.makeText(getApplicationContext(), "Thanks", Toast.LENGTH_SHORT).show();
                }
            }.start();
            mTimerRunning = true;

        } */

        stopcook.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mCountDownTimer.cancel();
                mTimerRef2= FirebaseDatabase.getInstance().getReference(actmod).child(burnerinuse).child("req_to_start_stop");
                mTimerRef.setValue(0);
                fl =new Intent(timeractivity.this,MainActivity.class);
                startActivity(fl);
                stop = true;
                Log.d("m1val34","is1"+minutes);
                Log.d("m1value35","is2"+seconds);
                Log.d("m1val36","is2"+m2);




            }
        });

        finishcook.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mTimerRef.setValue(0);
                mTimerRef4.setValue(0);
                fl = new Intent(timeractivity.this, MainActivity.class);
                AlertDialog.Builder altdial = new AlertDialog.Builder(timeractivity.this);
                altdial.setMessage("Has the food been cooked optimally").setCancelable(false)
                        .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                mTimerRef3.setValue("true");

                                startActivity(fl);

                            }
                        })
                        .setNegativeButton("No", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                mTimerRef3.setValue("false");

                                startActivity(fl);
                            }
                        });
                AlertDialog alert=altdial.create();
                alert.setTitle("Feedback");
                alert.show();

            }
        });


        cookmore.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

               // mTimerRef4.setValue(1);
                Log.d("nowprob",burnerinuse);
                fl2 =new Intent(timeractivity.this,cookextra.class);
                fl2.putExtra("themodel",actmod);
                fl2.putExtra("burnerinuse","Burner1");
                fl2.putExtra("item",food);
                startActivity(fl2);


            }
        });








    }

    public void assignvalues()
    {



        Intent f2=getIntent();
        burnerinuse = f2.getStringExtra("burnerinuse");
        actmod = f2.getStringExtra("themodel");
        food=f2.getStringExtra("item");

//        mTimerRef = FirebaseDatabase.getInstance().getReference("User").child("testuser").child("Burner1").child("time_selected");
//
//        mTimerRef.addValueEventListener(new ValueEventListener() {
//            @Override
//            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
//               mTimeLeftInMillis1 =(dataSnapshot.getValue(Long.class));
//
//                counttime(mTimeLeftInMillis1);
//
//
//
//            }
//
//            @Override
//            public void onCancelled(@NonNull DatabaseError databaseError) {
//                throw databaseError.toException();
//            }
//        });

    }

    public void counttime(long m1) {
        if(mCountDownTimer!=null)
        {
            mCountDownTimer.cancel();
        }

        mCountDownTimer = new CountDownTimer(m1, 1000) {
            @Override
            public void onTick(long millisUntilFinished) {
//               final long m1= millisUntilFinished;
                long m1= millisUntilFinished;
                m2 = m1;
                Log.d("t;hem2","the"+m2);
                updateCountDownText(m1);
            }

            @Override
            public void onFinish() {
                mTimerRef.setValue(0);
                mTimerRunning = false;
                 stopcook.setVisibility(View.GONE);
                cookmore.setVisibility(View.VISIBLE);
               finishcook.setVisibility(View.VISIBLE);
                //when timer will finish
                //submit marks to firebase and close this activity
                //once marks is been submitted to firebase then close the quiz

            }
        }.start();
        mTimerRunning = true;

    }







    public void updateCountDownText(long m1) {
//         minutes = (int) (m1 / 1000) / 60;
//         seconds = (int) (m1/ 1000) % 60;

        Log.d("m1value","is"+m1);

        minutes = (m1 / 1000) / 60;
        seconds = (m1/ 1000) % 60;
        if(stop == false){
            Log.d("m1value4","is1"+m1);
            Log.d("m1value1","is1"+minutes);
            Log.d("m1value2","is2"+seconds);
        }


        String timeLeftFormatted = String.format(Locale.getDefault(), "%02d:%02d", minutes, seconds);
        if(stop == false){
            Log.d("m1value3","is2"+timeLeftFormatted);
        }

        mQnATimer.setText(timeLeftFormatted);
    }

}