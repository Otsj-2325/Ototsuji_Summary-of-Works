/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * 弾の反射アルゴリズム・弾の基底クラス
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody2D))]
[RequireComponent(typeof(CircleCollider2D))]

public class Object0501_Bullet : System1001_CharacterBaseClass
{
    [Header("attackLevel * hitStopFrame")][SerializeField] protected int hitStopFrame = 0;
    private bool canReflection = true;
    [SerializeField]public bool safetyBullet = false;
    private int nextRefCnt = 0;
    [SerializeField] private int nextRefTime = 0;


    [SerializeField] protected int breakCntLimit = 0;
    [SerializeField] protected List<float> accMagniList;
    [SerializeField] protected float speedLimit = 30.0f;
    [SerializeField] protected PhysicsMaterial2D bulletPhysicMaterial = null;
    [SerializeField] protected float counterDistance = 1.8f;

    [SerializeField] public int attackLevel = 0;
    public bool ignoreFlag = false;

    protected int breakCnt = 0;
    [SerializeField]protected float moveSpeed;
    protected float clampedSpeed = 0.0f;
    protected int amlCnt = 0;
    [SerializeField] protected int nowAmCnt = 0;
    [SerializeField] protected Vector2 moveVector = Vector2.zero;
    [SerializeField] private bool breakCntFlag = false;
    float dist1;
    float dist2;

    [SerializeField]bool isVertical = false;
    [SerializeField]bool isHorizontal = false;

    [SerializeField] float hitStopTime = 0.25f;

    public bool onDestroyTrigger = false;

    [SerializeField] List<Sprite> bulletSprite;
    int spriteLevel = 0;

    bool reflectIgnore = false;
    int rfIgnoreCnt = 0;

     [SerializeField] private int ignoreFrameCnt = 0;
    [Header("最小反射角度(degree)")] [SerializeField] private float minDeg = 15.0f;

    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();/*これは毎回書く*/

        this.gameObject.tag = "Bullet";/*これは毎回書く*/
        this.gameObject.layer = LayerMask.NameToLayer("Bullet");/*これは毎回書く*/
        rb2d = this.gameObject.GetComponent<Rigidbody2D>();
        col2d = this.GetComponent<CircleCollider2D>();/*これは毎回書く*/

        rb2d.sharedMaterial = bulletPhysicMaterial;
        rb2d.mass = 1;
        rb2d.drag = 0;
        rb2d.angularDrag = 0;
        rb2d.gravityScale = 0;

        col2d.isTrigger = false;

        amlCnt = accMagniList.Count;
        rb2d.velocity = moveVector;

        attackLevel = 0;
        spriteLevel = attackLevel;
        GetComponent<SpriteRenderer>().sprite = bulletSprite[spriteLevel];

        if (base.checkConfig()) {/*これは毎回書く*/
            if (rb2d.sharedMaterial == null) {
                Debug.Log(this.gameObject.name + " : Can`t set PhysicsMaterial2D on Rigidbody2D.");
                executionFlag = false;
            }
        }

    }

    // Update is called once per frame
    protected virtual void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        if (!executionFlag)/*これは毎回書く*/
        {
            Debug.Log(this.gameObject.name + " : Can`t execution.");
            return;
        }

        if (!canReflection)
        {
            ++nextRefCnt;
            if(nextRefCnt > nextRefTime)
            {
                nextRefCnt = 0;
                canReflection = true;
            }
        }

        GameObject plrObj = GameObject.Find("Player").gameObject;
        GameObject cColObj = GameObject.Find("Player").transform.Find("Shield").transform.Find("CounterCollision").gameObject;

        dist1 = (plrObj.transform.position - transform.position).magnitude;
        dist2 = (cColObj.transform.position - transform.position).magnitude;

        if (reflectIgnore)
        {
            rfIgnoreCnt++;

            if(rfIgnoreCnt > ignoreFrameCnt)
            {
                rfIgnoreCnt = 0;
                reflectIgnore = false;
            }

        }
        rb2d.velocity = moveVector * moveSpeed * accMagniList[nowAmCnt];



        breakCnt++;
        if (breakCnt > breakCntLimit)
        {
            BrokeBullet();
        }

        ChangeSprite();
        ChangeFace();

        if (onDestroyTrigger)
        {
            BrokeBullet();
        }
    }

    public Vector2 BulletRefrect(Vector3 contactPos, Vector3 normal)
    {
        if (reflectIgnore)
        {
            return moveVector;
        }

        Debug.Log(this.gameObject.name + "before calculate moveVector : " + moveVector);

        Vector3 v1 = -moveVector;
        float v2 = Vector3.Dot(normal, v1);
        Vector3 v3 = moveVector;
        Vector3 calVec = v3 + 2 * v2 * normal;

        moveVector.x = (float)calVec.x;
        moveVector.y = (float)calVec.y;
        
        Debug.Log(this.gameObject.name + "moveVector : " + moveVector);

        reflectIgnore = true;
        rfIgnoreCnt = 0;

        return moveVector;
    }

    public void SetBulletData(Vector2 vel, float ms, float ls)
    {
        this.Start();

        rb2d.gravityScale = 0.0f;
        moveVector = vel;
        moveSpeed = ms;
        speedLimit = ls;        
    }

    public void BrokeBullet()
    {
        System1010_EffectManager.instance.EM_Explosion(this.transform.position, this.transform.rotation);
        Destroy(this.gameObject);
    }

    protected void ReflectionProc()
    {
        // 五十嵐　弾レベル3以上SE
        if (nowAmCnt >= 2)
        {
            Sound.instance.PlaySE(5);
        }

        if (nowAmCnt < amlCnt - 1)
        {
            if (nowAmCnt == 0)
            {
                // 五十嵐　弾レベル1SE
                Sound.instance.PlaySE(3);
            }
            else if (nowAmCnt == 1)
            {
                // 五十嵐　弾レベル2SE
                Sound.instance.PlaySE(4);
            }
            //else
            //{
            //    Sound.instance.PlaySE(5);
            //}
               

            nowAmCnt++;
        }

        attackLevel++;
    }

    public void MoveVelCalculation(Vector3 dir){

        if (reflectIgnore)
        {
            return;
        }

        moveVector = dir;

        reflectIgnore = true;
        rfIgnoreCnt = 0;
    }

    private void ChangeSprite()
    {
        if(spriteLevel != attackLevel)
        {
            spriteLevel++;

            if(spriteLevel > bulletSprite.Count - 1)
            {
                spriteLevel = bulletSprite.Count - 1;
            }

            GetComponent<SpriteRenderer>().sprite = bulletSprite[spriteLevel];
        }
    }

    private void ChangeFace()
    {
        Vector3 vel = moveVector;

        if(vel.x < 0)
        {
            vel.x *= -1;
        }
        if(vel.y < 0)
        {
            vel.y *= -1;
        }

        isVertical = (vel.x <= 0.15f);
        isHorizontal = (vel.y <= 0.15f);

        if(isVertical)
        {
            if(moveVector.y > 0)
            {//真上を向く
                var rot = this.transform.localEulerAngles;
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 270.0f;
                this.transform.localEulerAngles = rot;
            }
            else
            {//真下を向く
                var rot = this.transform.localEulerAngles;
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 90.0f;
                this.transform.localEulerAngles = rot;

            }
        }
        else if(isHorizontal)
        {
            if (moveVector.x > 0)
            {//右を向く
                var rot = this.transform.localEulerAngles;
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 180.0f;
                this.transform.localEulerAngles = rot;

            }
            else
            {//左を向く
                var rot = this.transform.localEulerAngles;
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 0.0f;
                this.transform.localEulerAngles = rot;

            }
        }
        else if(!isVertical && !isHorizontal)
        {
            if (moveVector.x > 00)
            {
                
                if (moveVector.y > 0)
                {
                    var rot = this.transform.localEulerAngles;
                    rot.x = 0.0f;
                    rot.y = 0.0f;
                    rot.z = 225.0f;
                    this.transform.localEulerAngles = rot;

                }
                else
                {
                    var rot = this.transform.localEulerAngles;
                    rot.x = 0.0f;
                    rot.y = 0.0f;
                    rot.z = 135.0f;
                    this.transform.localEulerAngles = rot;

                }
            }
            else
            {

                if (moveVector.y > 0)
                {
                    var rot = this.transform.localEulerAngles;
                    rot.x = 0.0f;
                    rot.y = 0.0f;
                    rot.z = -45.0f;
                    this.transform.localEulerAngles = rot;

                }
                else
                {
                    var rot = this.transform.localEulerAngles;
                    rot.x = 0.0f;
                    rot.y = 0.0f;
                    rot.z = 45.0f;
                    this.transform.localEulerAngles = rot;

                }
            }
        }
    }   

    public Vector2 GetMoveVec()
    {
        return moveVector;
    }

    private void BulletCounterProcess(Collider2D collision){
        
        GameObject cColObj = collision.gameObject;
        Character0112_CounterCollision ccs = cColObj.gameObject.GetComponent<Character0112_CounterCollision>();

        ReflectionProc();
        System1010_EffectManager.instance.EM_Counter(collision.ClosestPoint(this.transform.position), this.transform.rotation);


        Vector3 bulletPos = this.transform.position;
        Vector3 objPos = collision.transform.position;

        Vector3 dir = collision.gameObject.GetComponent<Character0112_CounterCollision>().shootDir;
        MoveVelCalculation(dir);

        ccs.IsCounter();//このタイミングでPlayerのカウンタージャンプの計算が走る

    }

    protected virtual void OnTriggerEnter2D(Collider2D collision)
    {
        /*BulletCounerProcess*/
        {
            if (collision.gameObject.tag == "CounterCollision")
            {
                if (!canReflection) return;

                moveVector = collision.GetComponent<Character0112_CounterCollision>().shootDir;
                BulletCounterProcess(collision);

                breakCnt = 0;

                //ヒットストップ(仮数値)
                HitStopSystem.instance.HitStop(attackLevel * hitStopFrame);
                canReflection = false;
            }
        }

        if (collision.gameObject.tag == "Coin_S")
        {
            if (attackLevel > 0)
            {
                // 五十嵐　コイン獲得SE
                Sound.instance.PlaySE(12);
                collision.gameObject.SetActive(false);
                Destroy(collision.gameObject);
                var cCnt = GameObject.Find("ItemCollector").gameObject.GetComponent<Character0106_ItemCollector>().sCnt;
                cCnt += 1;
                GameObject.Find("ItemCollector").gameObject.GetComponent<Character0106_ItemCollector>().sCnt = cCnt;
            }
        }

        if (collision.gameObject.tag == "Coin_B")
        {
            if (attackLevel > 0)
            {
                collision.gameObject.SetActive(false);
                Destroy(collision.gameObject);
                var cCnt = GameObject.Find("ItemCollector").gameObject.GetComponent<Character0106_ItemCollector>().sCnt;
                cCnt += 1;
                GameObject.Find("ItemCollector").gameObject.GetComponent<Character0106_ItemCollector>().sCnt = cCnt;
            }
        }
    }

    protected virtual void OnCollisionEnter2D(Collision2D collision)
    {        
        if (collision.gameObject.tag == "Ground" || collision.gameObject.tag == "Wall" || collision.gameObject.tag == "Ceiling" 
            || collision.gameObject.tag == "Block" || collision.gameObject.tag == "CounterChara" || collision.gameObject.tag == "DamageChara"
            || collision.gameObject.tag == "Switch" || collision.gameObject.tag == "Object")
        {
            if (nowAmCnt == 0)
            {
                System1010_EffectManager.instance.EM_Explosion(this.transform.position, this.transform.rotation);

                BrokeBullet();
            }
            else
            {
                // 五十嵐　壁反射SE
                Sound.instance.PlaySE(11);
                BulletRefrect(collision.GetContact(0).point, collision.GetContact(0).normal);
            }
        }

        if(collision.gameObject.tag == "Step")
        {
            if(nowAmCnt == 0)
            {
                System1010_EffectManager.instance.EM_Explosion(this.transform.position, this.transform.rotation);

                BrokeBullet();
            }
            else
            {
                canReflection = true;
            }
        }

        if(collision.gameObject.tag == "DarumaBody")
        {
            BulletRefrect(collision.GetContact(0).point, collision.GetContact(0).normal);
            canReflection = true;
        }

        if (collision.gameObject.tag == "BulletLiveTrigger"){
            breakCntFlag = false;
        }

        /*KillEnemyProcess*/
        {   
            if (collision.gameObject.tag == "Risu")
            {
                if (attackLevel > 0)
                {
                    if (!collision.gameObject.GetComponent<Character0211_NormalEnemy>().deathFlag)
                    {
                        System1010_EffectManager.instance.EM_Explosion(this.transform.position, this.transform.rotation);

                        var vec = collision.GetContact(0).point;
                        vec.y -= 1.0f;


                        collision.gameObject.GetComponent<Character0211_NormalEnemy>().calculateVector(vec, collision.GetContact(0).normal);
                        BrokeBullet();

                    }
                }
                else
                {
                    System1010_EffectManager.instance.EM_Explosion(this.transform.position, this.transform.rotation);
                    BrokeBullet();

                }
            }
        }
    }

    private void OnBecameInvisible()
    {
        BrokeBullet();
    }
}
