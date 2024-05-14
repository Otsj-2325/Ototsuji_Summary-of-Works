/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * プレイキャラ地面着地判定
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Character0103_GroundTrigger : MonoBehaviour
{
    [SerializeField]private bool isStand = false;
    
    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Ground")
        {
            isStand = true;
        }

        if (collision.gameObject.tag == "Step")
        {
            isStand = true;
        }

        if (collision.gameObject.tag == "Block")
        {
            isStand = true;
        }

        if (collision.gameObject.tag == "CounterChara")
        {
            isStand = true;
        }

        if (collision.gameObject.tag == "DamageChara")
        {
            isStand = true;
        }

        if (collision.gameObject.tag == "Object")
        {
            isStand = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Ground")
        {
            isStand = false;
        }

        if (collision.gameObject.tag == "Step")
        {
            isStand = false;
        }
        if (collision.gameObject.tag == "Block")
        {
            isStand = false;
        }

        if (collision.gameObject.tag == "CounterChara")
        {
            isStand = false;
        }

        if (collision.gameObject.tag == "DamageChara")
        {
            isStand = false;
        }

        if (collision.gameObject.tag == "Object")
        {
            isStand = false;
        }
    }
   
    public bool GetisStand()
    {
        return isStand;
    }
}
