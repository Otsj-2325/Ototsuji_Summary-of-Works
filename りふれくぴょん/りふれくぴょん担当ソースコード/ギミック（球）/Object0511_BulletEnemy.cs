/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * プレイヤーによる反射があるまではダメージ判定のある弾（敵キャラ用）
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Object0511_BulletEnemy : Object0501_Bullet
{
    protected override void Start()
    {
        base.Start();

        col2d.isTrigger = false;
    }

    protected override void OnTriggerEnter2D(Collider2D collision)
    {
        base.OnTriggerEnter2D(collision);

        
    }

    protected override void OnCollisionEnter2D(Collision2D collision)
    {
        base.OnCollisionEnter2D(collision);

        if(collision.gameObject.tag == "DarumaBody")
        {
            System1010_EffectManager.instance.EM_Explosion(this.transform.position, this.transform.rotation);

            BrokeBullet();
        }
    }
}
