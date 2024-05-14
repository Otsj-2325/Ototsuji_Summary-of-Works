/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * èÇÇÃîΩéÀìñÇΩÇËîªíËìÆçÏèàóù
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Character0112_CounterCollision : MonoBehaviour
{
    public Vector2 shootDir = Vector2.zero;

    [SerializeField] private string playerName = null;
    [SerializeField]public bool activate = false;
    private GameObject playerObj = null;
    private GameObject shield = null;
    private BoxCollider2D bC;

    private void Start()
    {
        playerObj = GameObject.Find(playerName).gameObject;

        shield = transform.parent.gameObject;
        bC = GetComponent<BoxCollider2D>();

        bC.enabled = false;
    }

    private void Update()
    {
        if(activate){
            bC.enabled = true;

        }
        else
        {
            bC.enabled = false;
        }
    }

    public void IsCounter()
    {
        shield.GetComponent<Character0111_Shield>().IsUnderCounter();
    }

    public bool IsVerticalX()
    {
        return shield.GetComponent<Character0111_Shield>().shieldVerticalX;
    }

    public bool IsVerticalY()
    {
        return shield.GetComponent<Character0111_Shield>().shieldVerticalY;
    }

    public void ControllerVibecation(Vector2 motorPower, int time)
    {
        shield.GetComponent<Character0111_Shield>().ControllerVibecation(motorPower, time);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag == "Bullet"){
            activate = false;
        }

    }
}
