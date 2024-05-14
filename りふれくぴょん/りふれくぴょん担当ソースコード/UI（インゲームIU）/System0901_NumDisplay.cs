using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class System0901_NumDisplay : MonoBehaviour
{
    [SerializeField] List<Sprite> numberSprite;
    [SerializeField] GameObject leastNumberPos;
    [SerializeField] float margin;
    [SerializeField] public int display = 0;
    int old = -999;
    List<GameObject> displayObjects;

    [SerializeField]List<int> numbers;
    bool setSprite = false;
 
    // Start is called before the first frame update
    void Start()
    {
        foreach(Sprite s in numberSprite)
        {
            if(s == null)
            {
                setSprite = false;
                break;

            }

            setSprite = true;
        }

        leastNumberPos.GetComponent<Image>().color = new Color(1.0f,1.0f, 1.0f, 0.0f);
        numbers = new List<int>();
        displayObjects = new List<GameObject>();

    }

    // Update is called once per frame
    void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        if (!setSprite || old == display)
        {
            return;
        }

        if(old != display)
        {
            foreach(GameObject g in displayObjects)
            {
                Destroy(g);
            }
        }

        CalculateNumber();
        CreateDisplay();
    }


    public void CalculateNumber()
    {
        numbers = new List<int>();
        
        int temp = display;
        if (temp > 0)
        {
            while (temp > 0)
            {
                numbers.Add(temp % 10);
                temp = temp / 10;
              
            }
        }
        else
        {
            numbers.Add(0);
        }

        old = display;
        PlayerPrefs.SetInt(System0101_GameManager.instance.CollectCoinCounter, display);

        Debug.Log("coin number called");
    }

    private void CreateDisplay()
    {
        RectTransform baseTransform = leastNumberPos.GetComponent<RectTransform>();
        var pos = baseTransform.anchoredPosition;

        foreach(int n in numbers)
        {
            GameObject dispnum = new GameObject();
            displayObjects.Add(dispnum);
            dispnum.AddComponent<Image>();
            dispnum.GetComponent<Image>().sprite = numberSprite[n];

            dispnum.transform.parent = this.gameObject.transform;
            dispnum.AddComponent<RectTransform>();
            RectTransform tf = dispnum.GetComponent<RectTransform>();
            tf.anchoredPosition = pos;
            tf.localScale = baseTransform.localScale;
            pos.x -= margin;
        }
    }
}
