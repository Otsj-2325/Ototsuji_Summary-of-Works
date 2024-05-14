/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * プレイキャラ操作処理
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using AIE2D;

[RequireComponent(typeof(CapsuleCollider2D))]
[RequireComponent(typeof(Rigidbody2D))]
[RequireComponent(typeof(Animator))]

public class Character0101_Player : System1001_CharacterBaseClass
{
    //Serialize Inspector
    [Header("初期スポーン地点")] [SerializeField] private GameObject initiPos;
    [Header("スピード")] [SerializeField] private float speed = 0.0f;    
    [Header("プレイヤー物理マテリアル")] [SerializeField] private PhysicsMaterial2D playerMaterial;

    [Header("コントローラー入力最小値")] [SerializeField] private Vector2 inputMin = Vector2.zero;
    [Header("ジャンプ入力最小値")] [SerializeField] private float jumpMin = 0.0f;

    [Header("ジャンプ高さ")][SerializeField] private float jumpHeight = 0.0f;
    [Header("ジャンプ滞空時間 / 2")][SerializeField] private float whileJumpTime = 0.0f;
    [Header("ジャンプ量/F")] [SerializeField] private int jumpSpeed = 0;

    [Header("小ジャンプ割合")] [SerializeField] private float smallJumpPower = 0;

    [Header("カウンタージャンプ高さ (F)")] [SerializeField] private float cJumpHeight = 0;
    [Header("カウンタージャンプ滞空時間 / 2")][SerializeField] private float whileCJumpTime = 0.0f;
    [Header("カウンタージャンプ量/F")] [SerializeField] private float cJumpSpeed;

    //Public Variable
    [HideInInspector] public bool faceLeft {  set; get; } = false;
    [HideInInspector]public Vector2 moveVel { set; get; }
    public bool startCounterJump { set; get; } = false;
    public bool isStand = false;

    //Private Variable
    [SerializeField] GameObject headObj;
    Character0190_HeadTrigger headTrigger;
    [SerializeField] GameObject groundObj;
    Character0103_GroundTrigger groundTrigger;
    [SerializeField]GameObject shieldObj;
    Character0111_Shield shieldTrigger;
    [SerializeField] GameObject damageObj;
    Character0104_DamageTrigger damageTrigger;

    [SerializeField]private float jumpLevel;
    [SerializeField]private bool getJumpLv = false;
    [SerializeField]private float jumpStartPos;
    [SerializeField]private float jumpValuePerFrame;
    [SerializeField]public bool isJump = false;
    [SerializeField]public bool isJumpUpMove = false;
    [SerializeField]public bool isJumpDownMove = false;
    [SerializeField]public bool isJumpLandMove = false;
    [SerializeField] private float jumpCntFrame = 0;
    [SerializeField]public bool isCounterJump = false;
    [SerializeField]private float cJumpCntFrame = 0;

    [Header("コントローラー入力値")] [SerializeField] Vector2 serializePad;

    [SerializeField] float inputLv = 0.0f;
    [SerializeField] float oldInputY = 0.0f;
    [SerializeField] bool isInput = false;

    public bool isFall = false;
    public bool isDamage = false;
    public bool isCounter = false;//パリィ中移動防止

    [Header("カウンター時移動不可")] [SerializeField] bool notMove = false;
    [Header("カウンター時移動量低下")] [SerializeField] bool slowMove = false;
    [Header("カウンター時移動量低下")] [SerializeField] float magnification = 0.1f;
    [Header("カウンター時振り向き不可")] [SerializeField] bool notAround = false;

    public bool stopUpdate = false;

    private StaticAfterImageEffect2DPlayer ae2d = null;
    [Header("残像色")] [SerializeField]private List<Color> colorList = new List<Color>();
    [Header("常時残像")] [SerializeField] bool everyAE = false;
    protected override void Start()
    {
        base.Start();

        rb2d = this.GetComponent<Rigidbody2D>();
        rb2d.gravityScale = 1.0f;

        col2d = this.GetComponent<CapsuleCollider2D>();
        col2d.sharedMaterial = playerMaterial;
        col2d.isTrigger = false;

        if (headObj)
        {
            headTrigger = headObj.GetComponent<Character0190_HeadTrigger>();
        }

        if (groundObj)
        {
            groundTrigger = groundObj.GetComponent<Character0103_GroundTrigger>();
        }

        if (shieldObj)
        {
            shieldTrigger = shieldObj.GetComponent<Character0111_Shield>();
        }

        if (damageObj)
        {
            damageTrigger = damageObj.GetComponent<Character0104_DamageTrigger>();
        }

        base.checkConfig();

        if (initiPos != null)
        {
            this.transform.position = initiPos.transform.position;
        }

        ae2d = this.GetComponent<StaticAfterImageEffect2DPlayer>();
        if (!everyAE) ae2d.SetColorIfneeded(colorList[1]);
        else ae2d.SetColorIfneeded(colorList[0]);
    }

    private void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        if (!executionFlag)/*これは毎回書く*/
        {
            Debug.Log(this.gameObject.name + " : Can`t execution.");
            return;
        }

        if (Gamepad.current == null)
        {
            return;
        }

        if (stopUpdate)
        {
            rb2d.velocity = Vector2.zero;
            rb2d.gravityScale = 0.0f;
            return;
        }
        else
        {
            rb2d.gravityScale = 1.0f;
        }

        moveVel = Vector2.zero;

        isStand = groundTrigger.GetisStand();
        
        WalkCalculate();//移動処理

        ShieldFace();

        JumpInput_Legacy();//ジャンプ入力受付

        JumpProcess_Legacy();//ジャンプによる移動計算
        

        rb2d.velocity = new Vector2(moveVel.x, rb2d.velocity.y + moveVel.y);

        if (!groundTrigger.GetisStand() && !isJump && !isCounterJump)
        {
            isFall = true;
        }
        else
        {
            isFall = false;
        }

        if (damageTrigger.isHurt)
        {
            isDamage = true;
        }
        else
        {
            isDamage = false;
        }

        if (groundTrigger.GetisStand())
        {
            if (!everyAE) ae2d.SetColorIfneeded(colorList[1]);
        }

        serializePad = Gamepad.current.leftStick.ReadValue();
    }

    //歩いてる動き計算
    private void WalkCalculate()
    {
        if(shieldTrigger.GetComponent<Character0111_Shield>().GetIsGuard() ){
            return;
        }

        if (notMove && isCounter) return;

        /*左右移動処理*/
        var xVal = Gamepad.current.leftStick.ReadValue().x;

        if (xVal > inputMin.x || xVal < -inputMin.x)
        {
            //Sound.instance.PlaySE(0);
            Vector2 vel;
            vel.x = xVal * speed;
            vel.y = moveVel.y;
            moveVel = vel;
        }

        if (slowMove && isCounter) moveVel *= magnification;

        if (notAround && isCounter) return;
        /*身体を決まった方向に向ける*/
        if (moveVel.x > 0) { faceLeft = false; }
        if(moveVel.x < 0) { faceLeft = true; }
    }
    //ジャンプ関係の入力受付
    private void JumpInput_Legacy()
    {
        if (notMove && isCounter) return;

        if (!isJump && !isCounterJump)
        {
            if (groundTrigger.GetisStand())
            {
                if (Gamepad.current.leftStick.ReadValue().y > inputMin.y)
                {
                    // 五十嵐　ジャンプSE
                    Sound.instance.PlaySE(1);
                    isJump = true;
                    jumpLevel = 1.0f;
                    jumpStartPos = this.transform.position.y;
                    
                }
                else if (Gamepad.current.leftStick.ReadValue().y > jumpMin)
                {
                    // 五十嵐　ジャンプSE
                    Sound.instance.PlaySE(1);
                    isJump = true;
                    jumpLevel = smallJumpPower;
                    jumpStartPos = this.transform.position.y;

                }
            }
        }
    }

    //ジャンプの処理
    private void JumpProcess_Legacy()
    {
        if (startCounterJump && !isCounterJump)
        {
            isJump = false;
            isJumpUpMove = false;
            jumpCntFrame = 0;
            jumpLevel = 0;
            Vector2 vel;
            vel.x = moveVel.x;
            vel.y = 0.0f;
            moveVel = vel;
            // 五十嵐　カウンタージャンプSE
            Sound.instance.PlaySE(2);
            isCounterJump = true;
            jumpStartPos = this.transform.position.y;

        }


        if (isJump)
        {
            if (jumpCntFrame < whileJumpTime / 4)
            {
                isJumpUpMove = true;

            }
            else if (jumpCntFrame < whileJumpTime / 3)
            {
                isJumpUpMove = false;
                isJumpDownMove = true;

            }
            else if(jumpCntFrame < whileJumpTime / 1.7)
            {
                isJumpDownMove = false;
                isJumpLandMove = true;

            }
            else
            {
                isJumpUpMove = false;
                isJumpDownMove = false;
                isJumpLandMove = false;
            }

            jumpValuePerFrame = 2 * Mathf.PI / whileJumpTime;

            JumpCalculate();
            jumpCntFrame += (float)1 / jumpSpeed;
        }

        else if (isCounterJump)
        {
          

            Debug.Log("through");
            ae2d.SetColorIfneeded(colorList[0]);

            startCounterJump = false;

            jumpValuePerFrame = 2 * Mathf.PI / whileCJumpTime;

            CounterJumpCalculate();
            cJumpCntFrame += (float) 1 / cJumpSpeed;
        }
        else
        {
            isJump = false;
            isJumpLandMove = false;
            jumpCntFrame = 0;
            inputLv = 0.0f;
            jumpLevel = 0.0f;

            isJumpUpMove = false;
            isJumpDownMove = false;
            isJumpLandMove = false;

            isCounterJump = false;
            cJumpCntFrame = 0;
        }

        if (headTrigger)
        {
            if (headTrigger.isCheck)
            {
                isJump = false;
                isJumpLandMove = false;
                jumpCntFrame = 0;
                inputLv = 0.0f;
                jumpLevel = 0.0f;

                isJumpUpMove = false;
                isJumpDownMove = false;
                isJumpLandMove = false;

                isCounterJump = false;
                cJumpCntFrame = 0;
            }
        }
    }

    //ジャンプの動き計算
    //sinCurveで動かしている
    private void JumpCalculate()
    {
        Vector3 calPos = this.transform.position;

        calPos.y = (float)Mathf.Sin(jumpValuePerFrame * jumpCntFrame) * jumpHeight * jumpLevel + jumpStartPos;
        this.transform.position = calPos;

        bool a = groundTrigger.GetisStand() && jumpCntFrame > 10.0f;
        bool b = (jumpValuePerFrame * jumpCntFrame) > Mathf.PI;
        bool c = this.gameObject.transform.position.y <= jumpStartPos;

        if (a || b || c)
        {
            isJump = false;
            isJumpLandMove = false;
            jumpCntFrame = 0;
            inputLv = 0.0f;
            jumpLevel = 0.0f;

            isJumpUpMove = false;
            isJumpDownMove = false;
            isJumpLandMove = false;
        }
    }

    //カウンタージャンプの動き計算
    private void CounterJumpCalculate()
    {

        Vector3 calPos = this.transform.position;

        calPos.y = (float)Mathf.Sin(jumpValuePerFrame * cJumpCntFrame) * cJumpHeight + jumpStartPos;
        this.transform.position = calPos;

        bool a = groundTrigger.GetisStand();
        bool b = (jumpValuePerFrame * cJumpCntFrame) > Mathf.PI;
        bool c = this.gameObject.transform.position.y <= jumpStartPos - 0.5f;
        bool d = shieldTrigger.GetComponent<Character0111_Shield>().GetIsCounter();
        bool e = (jumpValuePerFrame * cJumpCntFrame) > Mathf.PI / 2;


        if (a || b || c || (d && e))
        {
            isCounterJump = false;
            cJumpCntFrame = 0;
        }
    }

    //ガードしたら盾の向きに体を向けるやつ
    private void ShieldFace(){
        if (notAround && isCounter) return;

        if (shieldTrigger.GetComponent<Character0111_Shield>().GetIsGuard())
        {
            faceLeft = shieldTrigger.GetComponent<Character0111_Shield>().GetShieldFaceLeft();
        }

        ChangeFaceDirection();
    }

    //歩いてる向きに体を向ける
    private void ChangeFaceDirection()
    {
        if(isJump || isCounterJump){
            return;
        }

        /*向きの切り替え*/
        if (faceLeft)
        {
           transform.eulerAngles = new Vector3(0.0f, 180.0f, 0.0f);
        }
        else
        {
            transform.eulerAngles = new Vector3(0.0f, 0.0f, 0.0f);
        }
    }

    private void JumpInput()
    {
        if(!isJump && !isCounterJump){
            var yVal = Gamepad.current.leftStick.ReadValue().y;

            if (isInput)
            {
                inputLv = oldInputY - yVal;
                isInput = false;
            }

            if (yVal != 0.0f && !isInput)
            {
                isInput = true;
            }

            oldInputY = yVal;
        }

        {
            if (!isInput && inputLv != 0.0f)
            {
                if(inputLv > inputMin.y)
                {
                    isJump = true;
                    jumpStartPos = transform.position.y;

                    jumpLevel = 1.0f;
                }
                else if(inputLv > jumpMin)
                {
                    isJump = true;
                    jumpStartPos = transform.position.y;

                    jumpLevel = smallJumpPower;
                }
            }
        }
    }
    private void JumpProcess()
    {
        //カウンタージャンプの開始処理
        //ジャンプ中のカウンタージャンプ開始の時のジャンプ中断
        if (startCounterJump && !isCounterJump)
        {
            Debug.Log(this.gameObject.name + "start counterJump !!");
            isJump = false;
            jumpCntFrame = 0;
            jumpLevel = 0;
            Vector2 vel;
            vel.x = moveVel.x;
            vel.y = 0.0f;
            moveVel = vel;

            cJumpCntFrame = 0;
            isCounterJump = true;
            jumpStartPos = this.transform.position.y;

        }


        jumpValuePerFrame = 2 * Mathf.PI / whileJumpTime;

        //ジャンプ計算
        if (isJump)
        {
            JumpCalculate();
            jumpCntFrame += (float)1 / jumpSpeed;
        }

        //カウンタージャンプ計算
        if (isCounterJump)
        {
            startCounterJump = false;
            CounterJumpCalculate();
            cJumpCntFrame++;
        }
    }
    private void JumpCalculate_Legacy()
    {
        Vector3 calPos = this.transform.position;

        calPos.y = Mathf.Sin(jumpValuePerFrame * jumpCntFrame) * jumpHeight * jumpLevel + jumpStartPos;
        this.transform.position = calPos;

        bool a = groundTrigger.GetisStand() && jumpCntFrame > 10.0f;
        bool b = (jumpValuePerFrame * jumpCntFrame) > 2 * Mathf.PI;
        bool c = this.gameObject.transform.position.y <= jumpStartPos;

        if (a || b || c)
        {
            isJump = false;
            jumpCntFrame = 0;
            jumpLevel = 0;

            isJumpUpMove = false;
            isJumpDownMove = false;
            isJumpLandMove = false; 
        }
    }
}
