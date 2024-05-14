using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]


public class Character0108_Animator : MonoBehaviour
{
    enum PlayerAnimeState{ 
        Idle = 0,
        Walk,
        Jump,
        Guard,
        Counter,
        Victory,
    };

    [SerializeField]private PlayerAnimeState animeState = PlayerAnimeState.Idle;

    private Animator anime;
    private Character0101_Player scrPlayer;
    private Character0111_Shield scrShield;

    [SerializeField] private string nameIdle = "Character0100_Idle_S";
    [SerializeField] private string nameIdleWait1 = "Character0100_Idle_Wait1";
    [SerializeField] private string nameIdleWait2 = "Character0100_Idle_Wait2";
    [SerializeField] private string nameIdleWait3 = "Character0100_Idle_Wait3";
    [SerializeField] private string nameIdleWait4 = "Character0100_Idle_Wait4";
    [SerializeField] private string nameWalk = "Character0100_Walk_S";
    [SerializeField] private string nameJumpUp = "Character0100_JumpUp_S";
    [SerializeField] private string nameJumpDown = "Character0100_JumpDown_S";
    [SerializeField] private string nameJumpLand = "Character0100_JumpLand_S";
    [SerializeField] private string nameUpperCounter = "Character0100_UpperCounter";
    [SerializeField] private string nameLowerCounter = "Character0100_LowerCounter";
    [SerializeField] private string nameHorizontalCounter = "Character0100_HorizontalCounter";
    [SerializeField] private string nameUpperCharge = "Character0100_UpperCharge";
    [SerializeField] private string nameLowerCharge = "Character0100_LowerCharge";
    [SerializeField] private string nameHorizontalCharge = "Character0100_HorizontalCharge";
    [SerializeField] private string nameCJumpCharge = "Character0100_CJumpCharge";
    [SerializeField] private string nameCounterJump = "Character0100_CounterJump";
    [SerializeField] private string nameUpCharge = "Character0100_UpCharge";
    [SerializeField] private string nameUpCounter = "Character0100_UpCounter";
    [SerializeField] private string nameVictory = "Character0100_Victory";
    [SerializeField] private string nameDamage1 = "Character0100_Damage1";
    [SerializeField] private string nameDamage2 = "Character0100_Damage2";

    private int hashIdle;
    private int hashIdleWait1;
    private int hashIdleWait2;
    private int hashIdleWait3;
    private int hashIdleWait4;
    private int hashWalk;
    private int hashJumpUp;
    private int hashJumpDown;
    private int hashJumpLand;
    private int hashUpperCounter;
    private int hashLowerCounter;
    private int hashHorizontalCounter;
    private int hashUpperCharge;
    private int hashLowerCharge;
    private int hashHorizontalCharge;
    private int hashCJumpCharge;
    private int hashCounterJump;
    private int hashUpCharge;
    private int hashUpCounter;
    private int hashVictory;
    private int hashDamage1;
    private int hashDamage2;

    private float timeToWaitForVictorey = 1.5f;

    [SerializeField]AnimatorStateInfo currentInfo;
    [SerializeField]bool isCounter = false;
    [SerializeField] bool isVictory = false;

    [SerializeField] int translateWaitCnt = 180;
    [SerializeField] int waitCnt = 0;
    [SerializeField] bool isWait = false;
    [SerializeField] bool isFall = false;
    [SerializeField] bool isDamage = false;
    int damageAnimeCnt = 0;


    private void Start()
    {
        anime = this.gameObject.GetComponent<Animator>();

        hashIdle = Animator.StringToHash(nameIdle);
        hashIdleWait1 = Animator.StringToHash(nameIdleWait1);
        
        hashIdleWait2 = Animator.StringToHash(nameIdleWait2);
        hashIdleWait3 = Animator.StringToHash(nameIdleWait3);
        hashIdleWait4 = Animator.StringToHash(nameIdleWait4);
        hashWalk = Animator.StringToHash(nameWalk);
        hashJumpUp = Animator.StringToHash(nameJumpUp);
        hashJumpDown = Animator.StringToHash(nameJumpDown);
        hashJumpLand = Animator.StringToHash(nameJumpLand);
        hashUpperCharge = Animator.StringToHash(nameUpperCharge);
        hashUpperCounter = Animator.StringToHash(nameUpperCounter);
        hashLowerCharge = Animator.StringToHash(nameLowerCharge);
        hashLowerCounter = Animator.StringToHash(nameLowerCounter);        
        hashHorizontalCounter = Animator.StringToHash(nameHorizontalCounter);
        hashHorizontalCharge = Animator.StringToHash(nameHorizontalCharge);
        hashCJumpCharge = Animator.StringToHash(nameCJumpCharge);
        hashCounterJump = Animator.StringToHash(nameCounterJump);
        hashUpCharge = Animator.StringToHash(nameUpCharge);
        hashUpCounter = Animator.StringToHash(nameUpCounter);
        hashVictory = Animator.StringToHash(nameVictory);        
        hashDamage1 = Animator.StringToHash(nameDamage1);        
        hashDamage2 = Animator.StringToHash(nameDamage2);

        scrPlayer = this.gameObject.GetComponent<Character0101_Player>();
        scrShield = transform.Find("Shield").gameObject.GetComponent<Character0111_Shield>();

        anime.Play(hashIdle);
    }

    private void Update()
    {
        if(Time.timeScale == 0) { return; }

        currentInfo = anime.GetCurrentAnimatorStateInfo(0);
        
        if(animeState == PlayerAnimeState.Idle)
        {
            if(!currentInfo.IsName(nameIdle) && !isWait && !isFall && !isDamage)
            {
                anime.Play(hashIdle);
                waitCnt = 0;
            }
            if (scrShield.GetIsGuard())
            {
                animeState = PlayerAnimeState.Guard;
            }
            if (scrPlayer.isJumpUpMove)
            {
                animeState = PlayerAnimeState.Jump;
            }
            if(scrPlayer.moveVel.x != 0.0f)
            {
                animeState = PlayerAnimeState.Walk;
            }

            if (scrPlayer.isDamage)
            {
                DamageAnimeProc();

            }
            else if (isDamage)
            {
                damageAnimeCnt++;

                if (damageAnimeCnt > 60)
                {
                    isDamage = false;
                }
            }

            if (scrPlayer.isFall)
            {
                FallAnimeProc();
            }
            else
            {
                isFall = false;
            }

            IdleWaitProc();

        }
        else if(animeState == PlayerAnimeState.Walk)
        {
            if(!currentInfo.IsName(nameWalk))
            {

                anime.Play(hashWalk);
            }

            if (scrPlayer.moveVel.x == 0.0f)
            {
                animeState = PlayerAnimeState.Idle;
            }

            if (scrPlayer.isJumpUpMove)
            {
                animeState = PlayerAnimeState.Jump;
            }
        }
        else if(animeState == PlayerAnimeState.Guard)
        {
            // チャージ中の方向のプルプル再生
            if (scrShield.chargeUpperNow && !currentInfo.IsName(nameUpperCharge))//斜め上プルプル
            {
                anime.Play(hashUpperCharge);
            }
            else if (scrShield.chargeLowerNow && !currentInfo.IsName(nameLowerCharge))//斜め下プルプル
            {
                anime.Play(hashLowerCharge);
            }
            else if (scrShield.chargeHorizontalNow && !currentInfo.IsName(nameHorizontalCharge))//水平プルプル
            {
                anime.Play(hashHorizontalCharge);
            }
            else if (scrShield.cJumpCharge && !currentInfo.IsName(nameCJumpCharge))//カウンタージャンプ準備
            {
                anime.Play(hashCJumpCharge);
                                
            }
            else if (scrShield.verticalDown && !currentInfo.IsName(nameUpCharge))//真上プルプル
            {
                anime.Play(hashUpCharge);

            }

            if (scrShield.resetMode)
            {
                animeState = PlayerAnimeState.Idle;
            }
            
            if (scrShield.GetIsCounter())
            {
                animeState = PlayerAnimeState.Counter;
            }
        }
        else if(animeState == PlayerAnimeState.Counter)
        {
            if (!isCounter)
            {
                StartCoroutine(CoutnerAnimeProc());
            }

        }
        else if(animeState == PlayerAnimeState.Jump)
        {
            if (scrPlayer.isJumpUpMove && !currentInfo.IsName(nameJumpUp))
            {
                anime.Play(hashJumpUp);
            }
            else if (scrPlayer.isJumpLandMove && !currentInfo.IsName(nameJumpLand))
            {
                anime.Play(hashJumpLand);
            }
            else if (scrPlayer.isJumpDownMove && !currentInfo.IsName(nameJumpDown))
            {
                anime.Play(hashJumpDown);

            }

            bool q = !scrPlayer.isJumpUpMove && !scrPlayer.isJumpDownMove && !scrPlayer.isJumpLandMove;

            if (q)
            {
                animeState = PlayerAnimeState.Idle;
            }

            if (scrShield.GetIsGuard())
            {
                animeState = PlayerAnimeState.Guard;
            }
        }
        else if(animeState == PlayerAnimeState.Victory)
        {
            if (!isVictory)
            {
                StartCoroutine(VictoryAnimeProc());
            }
        }


        
    }

    private void IdleWaitProc()
    {
        if(waitCnt > translateWaitCnt && !isWait)
        {
            StartCoroutine(WaitAnimationProc());
            isWait = true;
        }

        waitCnt++;
    }

    private void FallAnimeProc()
    {
        if (!currentInfo.IsName(nameJumpDown))
        {
            anime.Play(hashJumpDown);
            isFall = true;
        }
    }

    private void DamageAnimeProc()
    {
        if (!currentInfo.IsName(nameJumpDown))
        {
            if(Random.Range(1, 100) > 80)
            {
                anime.Play(hashDamage2);

            }
            else
            {
                anime.Play(hashDamage1);

            }

            isDamage = true;
            damageAnimeCnt = 0;
        }
        
    }

    private IEnumerator WaitAnimationProc()
    {
        anime.Play(hashIdleWait1);
        yield return null;
        yield return new System1002_WaitForAnimation(anime, 0);

        if(Random.Range(1, 100) > 90)
        {
            anime.Play(hashIdleWait2);
            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);

            anime.Play(hashIdleWait3);
            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);
        }
        else
        {
            anime.Play(hashIdleWait3);
            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);
        }

        anime.Play(hashIdleWait4);
        yield return null;
        yield return new System1002_WaitForAnimation(anime, 0);

        waitCnt = -1800;
        isWait = false;
    }

    private IEnumerator CoutnerAnimeProc(){

        isCounter = true;

        // 斜め上のアニメーション
        if ((scrShield.upperLeft || scrShield.upperRight) && !currentInfo.IsName(nameUpperCounter))
        {
            anime.Play(hashUpperCounter);
            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);

            animeState = PlayerAnimeState.Idle;
        }
        // 斜め下のアニメーション
        else if ((scrShield.lowerLeft || scrShield.lowerRight) && !currentInfo.IsName(nameLowerCounter))
        {
            anime.Play(hashLowerCounter);
            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);

            animeState = PlayerAnimeState.Idle;
        }
        // 水平方向のアニメーション
        else if (scrShield.shieldVerticalX && !currentInfo.IsName(nameHorizontalCounter))
        {
            anime.Play(hashHorizontalCounter);
            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);

            animeState = PlayerAnimeState.Idle;
        }
        else if (scrShield.verticalDown && !currentInfo.IsName(nameUpCounter))
        {
            anime.Play(hashUpCounter);
            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);

            animeState = PlayerAnimeState.Idle;
        }
        else if (scrShield.counterJump && !currentInfo.IsName(nameCounterJump))
        {
            anime.Play(hashCounterJump);

            yield return null;
            yield return new System1002_WaitForAnimation(anime, 0);

            animeState = PlayerAnimeState.Idle;
        }
        else if (scrShield.resetMode)
        {
            animeState = PlayerAnimeState.Idle;

        }

        isCounter = false;
    }

    IEnumerator VictoryAnimeProc()
    {
        isVictory = true;
        yield return new WaitForSecondsRealtime(timeToWaitForVictorey);
        anime.Play(hashVictory);
        yield return null;
        yield return new System1002_WaitForAnimation(anime, 0);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Goal")
        {
            animeState = PlayerAnimeState.Victory;
        }
    }

}
