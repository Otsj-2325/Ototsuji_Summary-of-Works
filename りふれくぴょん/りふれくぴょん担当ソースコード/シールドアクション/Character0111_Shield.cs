/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * 盾の基本動作処理
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class Character0111_Shield : MonoBehaviour
{
    [Header("発生○○F.ver")] [SerializeField] bool delay = false;
    [Header("発生frame")] [SerializeField] private int delayTime = 10;



    private GameObject player = null;
    private Character0101_Player scrPlayer = null;
    private GameObject cCollision;
    private Character0112_CounterCollision scrCCollision;
    private bool isShieldFaceLeft = false;

    [SerializeField]private int CounterDrectionCut = 8;
    [SerializeField] private float degree = 0.0f;
    private float splitAngle = 0.0f;
    [SerializeField] private float nowAngle = 0.0f;

    public bool isNeutral = false;
    public bool isGuard = false;
    public bool isKeepGuard = false;
    public bool isCounter = false;
    public bool isKeepCounter = false;

    public bool shieldVerticalX = false;
    public bool shieldVerticalY = false;

    public bool cJumpCharge = false;
    public bool counterJump = false;
    public bool verticalDown = false;
    public bool upperLeft = false;
    public bool upperRight = false; 
    public bool lowerLeft = false; 
    public bool lowerRight = false;
    public bool chargeUpperNow = false;
    public bool chargeLowerNow = false;
    public bool chargeHorizontalNow = false;

    [SerializeField] private int counterTime;
    private int counterCount = 0;
    private int TimeCount = 0;
    private int vibecationTime = 0;
    private bool canVibecation = true;
    private Vector2 readStickValue = Vector2.zero;

    [SerializeField]int state = 0;
    bool changeMode = false;

    public bool resetMode = false; 

    // Start is called before the first frame update
    void Start()
    {
        player = transform.parent.gameObject;
        scrPlayer = player.GetComponent<Character0101_Player>();
        cCollision = this.transform.Find("CounterCollision").gameObject;
        scrCCollision = cCollision.GetComponent<Character0112_CounterCollision>();
        splitAngle = 360 / CounterDrectionCut;
    }

    // Update is called once per frame
    private void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        GetGamepad();
        
        shieldVerticalX = false;
        shieldVerticalY = false;

        upperLeft = false;
        upperRight = false;
        lowerLeft = false;
        lowerRight = false;

        chargeUpperNow = false;
        chargeLowerNow = false;
        chargeHorizontalNow = false;

        if (state == 0){
            NeutralMode();

            if(changeMode){
                state += 1;
                changeMode = false;

                isNeutral = false;
                isGuard = true;
            }
        }
        else if(state == 1){
            GuardMode();

            if (changeMode)
            {
                
                state += 1;
                changeMode = false;

                isGuard = false;
                isCounter = true;

                if(!cJumpCharge) scrPlayer.isCounter = true;//プレイヤ移動防止
                //カウンターコリジョン
                if(!delay) scrCCollision.activate = true;
            }
        }
        else if(state == 2){
            CounterMode();

            if (changeMode)
            {
                state = 0;
                changeMode = false;

                isCounter = false;
                isKeepCounter = false;
                isNeutral = true;

                scrPlayer.isCounter = false;

                scrCCollision.activate = false;
            }
        }


        //Flag managment by nowAngle
        transform.eulerAngles = new Vector3(0.0f, 0.0f, nowAngle);

        if (nowAngle == -90 || nowAngle == 90)
        {
            shieldVerticalY = true;
        }
        else
        {
            shieldVerticalY = false;
        }
        
        // 水平に溜めてるときのプルプル
        if(nowAngle == 0 || nowAngle == -180)
        {
            chargeHorizontalNow = true;
            shieldVerticalX = true;
        }
        
        // shieldVerticalYとshieldVerticalXがfalseの時
        if (!shieldVerticalY && !shieldVerticalX)
        {
            // 斜め上と斜め下のチャージ中のプルプル
            if (nowAngle == -135 || nowAngle == -45)
            {
                chargeUpperNow = true;
            }
            else if (nowAngle == -225 || nowAngle == 45)
            {
                chargeLowerNow = true;
            }

            if (!chargeUpperNow || !chargeLowerNow)
            {
                // 右斜め上判定、右斜め下判定
                if (nowAngle == -135)
                {
                    upperRight = true;
                }
                else if (nowAngle == -225)
                {
                    lowerRight = true;
                }
                // 左斜め上判定、左斜め下判定
                if (nowAngle == -45)
                {
                    upperLeft = true;
                }
                else if (nowAngle == 45)
                {
                    lowerLeft = true;
                }


            }

        }

        if (nowAngle >= -240 && nowAngle <= -120) { isShieldFaceLeft = false; }
        else if (nowAngle >= -60 && nowAngle <= 60) { isShieldFaceLeft = true; }

    }

    //スティックの角度計算
    private void GetGamepad()
    {
        if (Gamepad.current == null)
        {
            return;
        }

        readStickValue = Gamepad.current.rightStick.ReadValue();

        degree = Mathf.Atan2(readStickValue.x, readStickValue.y) * Mathf.Rad2Deg;

        if (degree < 0)
        {
            degree += 360;
        }
    }

    private void NeutralMode()
    {
        resetMode = false;

        if (readStickValue.x < -0.25f || readStickValue.y < -0.25f || readStickValue.x > 0.25f || readStickValue.y > 0.25f)
        {
            nowAngle = 90 - ((int)((degree + splitAngle / 2) / splitAngle) * splitAngle);

            if (nowAngle != 90 && nowAngle != -270 || !scrPlayer.isStand) { 
                changeMode = true;
            }
        }
    }

    private void GuardMode()
    {

        if (readStickValue.x < -0.3f || readStickValue.y < -0.3f || readStickValue.x > 0.3f || readStickValue.y > 0.3f)
        {
            cJumpCharge = false;
            verticalDown = false;

            nowAngle = 90 - ((int)((degree + splitAngle / 2) / splitAngle) * splitAngle);

        }

        Vector2 calVal = readStickValue;
        if (calVal.x < 0)
        {
            calVal.x *= -1.0f;
        }
        if (calVal.y < 0)
        {
            calVal.y *= -1.0f;
        }

        if (calVal.x < 0.1f && calVal.y < 0.1f)
        {

            changeMode = true;

            if (cJumpCharge)
            {
                cJumpCharge = false;
                counterJump = true;
            }

            if (verticalDown)
            {
                verticalDown = false;
            }
        }

        if (nowAngle == 0)
        {
            scrCCollision.shootDir = Vector2.left;

        }
        else if (nowAngle == 45)
        {
            scrCCollision.shootDir = new Vector2(-1.0f, -1.0f);

        }
        else if (nowAngle == 90 || nowAngle == -270)
        {
            scrCCollision.shootDir = Vector2.down;

            if (!cJumpCharge)
            {
                cJumpCharge = true;
            }

            if (scrPlayer.isStand)
            {
                state = 0;
                resetMode = true;
                isNeutral = true;
                isGuard = false;
            }
        }
        else if (nowAngle == -225)
        {
            scrCCollision.shootDir = new Vector2(1.0f, -1.0f);

        }
        else if (nowAngle == -180)
        {
            scrCCollision.shootDir = Vector2.right;

        }
        else if (nowAngle == -135)
        {
            scrCCollision.shootDir = new Vector2(1.0f, 1.0f);

        }
        else if (nowAngle == -90 || nowAngle == 270)
        {
            scrCCollision.shootDir = Vector2.up;

            if (!verticalDown)
            {
                verticalDown = true;
            }
        }
        else if (nowAngle == -45)
        {
            scrCCollision.shootDir = new Vector2(-1.0f, 1.0f);

        }
        else
        {
            scrCCollision.shootDir = Vector2.zero;

        }
    }

    private void CounterMode()
    {
        counterCount++;

        if (delay)
        {
            if (cJumpCharge)
            {
                if (counterCount >= (delayTime / 2)) scrCCollision.activate = true;
            }
            else
            {
                if (counterCount >= delayTime) scrCCollision.activate = true;
            }
        }

        if (counterCount > counterTime)
        {
            counterCount = 0;
            changeMode = true;

            if (counterJump)
            {
                cJumpCharge = false;
                counterJump = false;
            }

            if (verticalDown)
            {
                verticalDown = false;
            }
        }
    }

    public void IsUnderCounter()
    {
        if (nowAngle > 60 || nowAngle < -240)
        {
            player.GetComponent<Character0101_Player>().startCounterJump = true;
        }
    }

    public void ControllerVibecation(Vector2 motorPower, int time)
    {
        if (canVibecation)
        {
            Gamepad.current.SetMotorSpeeds(motorPower.x, motorPower.y);
            vibecationTime = time;
            TimeCount = 0;
            canVibecation = false;
        }
    }

    public bool GetShieldFaceLeft()
    {
        return isShieldFaceLeft;
    }

    public bool GetIsGuard()
    {
        return isGuard;
    }

    public bool GetIsCounter()
    {
        return isCounter;
    }
    
    private void ControllerVibecationStop()
    {
        Gamepad.current.SetMotorSpeeds(0.0f, 0.0f);
        vibecationTime = 0;
        canVibecation = true;
    }

}