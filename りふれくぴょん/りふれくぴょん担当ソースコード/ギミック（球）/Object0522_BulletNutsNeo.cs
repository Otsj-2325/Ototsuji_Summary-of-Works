using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Object0522_BulletNutsNeo : Object0501_Bullet
{
    [SerializeField] private float fallGravity = 0.0f;
    private bool counterFlag = false;
    [SerializeField] private bool darumaNuts = false;

    protected override void Start()
    {
        base.Start();

        rb2d.gravityScale = fallGravity;
        if (darumaNuts) { col2d.isTrigger = true; }
        else { col2d.isTrigger = false; }
       
        counterFlag = false;
        this.gameObject.layer = LayerMask.NameToLayer("Nuts");
    }

    protected override void Update()
    {
        if (counterFlag)
        {
            base.Update();
        }
    }

    protected override void OnTriggerEnter2D(Collider2D collision)
    {
        Debug.Log(this.gameObject.name + "Counter");

        if(!counterFlag && collision.gameObject.tag == "CounterCollision")
        {
            if(ignoreFlag){
                return;
            }

            System1010_EffectManager.instance.EM_Counter(collision.ClosestPoint(this.transform.position), this.transform.rotation);
            counterFlag = true;
            col2d.isTrigger = false;
            rb2d.gravityScale = 0.0f;
            this.gameObject.layer = LayerMask.NameToLayer("Bullet");
            
        }

        base.OnTriggerEnter2D(collision);
    }

    protected override void OnCollisionEnter2D(Collision2D collision)
    {
        base.OnCollisionEnter2D(collision);

    }

    private void OnBecameInvisible()
    {
        base.BrokeBullet();
    }
}
