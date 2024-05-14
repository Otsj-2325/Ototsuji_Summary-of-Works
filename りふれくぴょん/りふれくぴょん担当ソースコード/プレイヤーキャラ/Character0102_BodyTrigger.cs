/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * プレイキャラの盾と逆側から接近した弾の反射除外処理
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(BoxCollider2D))]

public class Character0102_BodyTrigger : System1001_CharacterBaseClass
{
    protected override void Start()
    {
        col2d = this.GetComponent<BoxCollider2D>();

        col2d.isTrigger = true;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag == "Bullet")
        {
            collision.gameObject.GetComponent<Object0501_Bullet>().ignoreFlag = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            collision.gameObject.GetComponent<Object0501_Bullet>().ignoreFlag = false; ;
        }
    }
}
