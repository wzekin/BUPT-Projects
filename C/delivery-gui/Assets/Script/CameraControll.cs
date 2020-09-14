using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControll : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Cursor.lockState = CursorLockMode.Confined; //固定鼠标于游戏内
    }
    //用于处理摄像机移动问题
    void Update()
    {
        //通过判断鼠标是否在边缘移动摄像机
        if (Input.mousePosition.x < 5)
        {
            if(transform.position.x > -4)
            {
                transform.position -= new Vector3(0.1F, 0, 0);
            }
        }
        if (Input.mousePosition.x > Screen.width - 5)
        {
            if(transform.position.x < 4)
            {
                transform.position += new Vector3(0.1F, 0, 0);
            }

        }
        if (Input.mousePosition.y < 5)
        {
            if (transform.position.z > -9)
            {
                transform.position -= new Vector3(0, 0, 0.1F);
            }
        }
        if (Input.mousePosition.y > Screen.height - 5)
        {
            if (transform.position.z < 1)
            {
                transform.position += new Vector3(0, 0, 0.1F);
            }
        }
        //捕捉鼠标滚轮响应拉近，拉远摄像机
        if (Input.GetAxis("Mouse ScrollWheel") != 0)//这个是鼠标滚轮响应函数
        {
            //滚轮响应一次就让scale自增或自减，注意，滚轮函数是有返回值的，
            //返回是float型的！这个由滚轮向前（正数）还是向后（负数）滚决定
            float scale = -Input.GetAxis("Mouse ScrollWheel");
            if ((scale > 0 && transform.position.y < 8) || (scale < 0 && transform.position.y > 2))
            {
                transform.position += new Vector3(0, scale, 0);
            }
        }
    }
}
