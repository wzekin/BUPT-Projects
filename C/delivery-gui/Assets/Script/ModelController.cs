using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class ModelController : MonoBehaviour
{
    //要操纵的文本框
    public InputField time;
    public InputField money;
    public InputField numAll;
    public InputField numFinished;
    public InputField numOutdated;

    //用来计数1s的时间戳
    private float time_;

    //主程序
    private static Model m;

    //用来自动添加的rider，coin，star Object
    private static GameObject rider;
    private static GameObject coin;
    private static GameObject star;

    //骑手Object riders
    private GameObject[] riders = new GameObject[100]; 

    //计算骑手中间位置
    private Point[] nextPoints = new Point[10];

    //用于标记餐馆食客的Object
    private static GameObject[,] points = new GameObject[18, 18];

    //骑手计数器
    private int riderCount = 0;

    //将Point坐标转换为Vector坐标
    private Vector3 pointToVector(Point p)
    {
        return new Vector3(p.y - 8, 0, -p.x + 8);
    }
    void Start()
    {
        //加载模型数据，初始化Model
        rider = (GameObject)Resources.Load("Prefab/MaleFreeSimpleMovement1");
        coin = (GameObject)Resources.Load("Prefab/Coin Variant");
        star = (GameObject)Resources.Load("Prefab/Star Variant");
        m = new Model(new Point(8, 9));
    }

    private void myFixedUpdate()
    {
        //更新文本信息
        time.text = m.time().ToString();
        money.text = m.money().ToString();
        numAll.text = m.num_all().ToString();
        numFinished.text = m.num_finished().ToString();
        numOutdated.text = m.num_outdate().ToString();
        //如果停靠，则删除停靠点的标记信息
        for (int i = 0; i < riderCount; i++)
        {
            foreach (Point p in m.riders[i].dock_points())
            {
                if (points[p.x, p.y] != null)
                {
                    points[p.x, p.y].GetComponent<RotateController>().Destroy();
                    Destroy(points[p.x, p.y], 0.5F);
                    points[p.x, p.y] = null;

                }
            }
        }
        //执行下一步操作
        m.Step();
        // 检测骑手变化并添加骑手
        if (riderCount != m.riders.Count)
        {
            riders[riderCount] = Instantiate(rider,
                pointToVector(m.riders[riderCount].position()),
                new Quaternion()); // 生成骑手
            riders[riderCount].transform.GetChild(0).GetComponentInChildren<TextMesh>().text = riderCount.ToString(); //标记第几个骑手
            nextPoints[riderCount] = m.riders[riderCount].position(); //初始化rider_count
            riderCount++;
        }

        //计算骑手的中间位置并转向
        for (int i = 0; i < riderCount; i++)
        {
            int delta_x = m.riders[i].position().x - nextPoints[i].x;
            int delta_y = m.riders[i].position().y - nextPoints[i].y;
            if (nextPoints[i].x % 2 == 0)
            {
                if (delta_x != 0)
                {
                    nextPoints[i].x += Math.Abs(delta_x) / delta_x;
                    riders[i].transform.rotation = Quaternion.Euler(0f, delta_x < 0 ? 0f : 180f, 0f);
                }
            }
            else
            {
                if (delta_y != 0)
                {
                    nextPoints[i].y += Math.Abs(delta_y) / delta_y;
                    riders[i].transform.rotation = Quaternion.Euler(0f, delta_y > 0 ? 90f : 270f, 0f);
                }
            }
        }

    }
    // Update is called once per frame
    void FixedUpdate()
    {
        //骑手移动
        for (int i = 0; i < riderCount; i++)
        {
            //判断骑手是否在移动并添加移动动画
            if (pointToVector(m.riders[i].position()) == riders[i].transform.position)
            {
                riders[i].GetComponent<SimpleCharacterControl>().m_animator.SetFloat("MoveSpeed", 0);
            }
            else
            {
                riders[i].GetComponent<SimpleCharacterControl>().m_animator.SetFloat("MoveSpeed", 0.33F);
            }
            //判断骑手是否到达中间位置，并根据下一步的位置转向
            if (pointToVector(nextPoints[i]) == riders[i].transform.position)
            {
                int delta_x = m.riders[i].position().x - nextPoints[i].x;
                int delta_y = m.riders[i].position().y - nextPoints[i].y;
                if (delta_x != 0)
                {
                    nextPoints[i].x += Math.Abs(delta_x) / delta_x;
                    riders[i].transform.rotation = Quaternion.Euler(0f, delta_x < 0 ? 0f : 180f, 0f);
                }
                if (delta_y != 0)
                {
                    nextPoints[i].y += Math.Abs(delta_y) / delta_y;
                    riders[i].transform.rotation = Quaternion.Euler(0f, delta_y > 0 ? 90f : 270f, 0f);
                }
            }
            //移动骑手
            riders[i].transform.position = Vector3.MoveTowards(
                riders[i].transform.position,
                pointToVector(nextPoints[i]),
                0.020F);
        }
        //每隔一秒更新文本信息，并进行下一步的运算
        if (time_ >= 1)
        {
            myFixedUpdate();
            time_ = 0;
        }
        time_ += Time.deltaTime;
    }
    //添加订单
    public void AddOrder(int id, int x1, int y1, int x2, int y2)
    {
        m.AddOrder(id, m.time(), x1, y1, x2, y2);
        //添加餐馆标记信息
        if (points[x1, y1] != null)
        {
            Destroy(points[x1, y1]);
        }
        points[x1, y1] = Instantiate(star, new Vector3(y1 - 8, 1.5F, 8 - x1), new Quaternion());
        //添加订单号
        TextMesh[] t = points[x1, y1].transform.GetComponentsInChildren<TextMesh>();
        t[0].text = System.String.Format("{0}", id);
        t[1].text = System.String.Format("{0}", id);
        //添加食客标记信息
        if (points[x2, y2] != null)
        {
            Destroy(points[x2, y2]);
        }
        points[x2, y2] = Instantiate(coin, new Vector3(y2 - 8, 1.5F, 8 - x2), new Quaternion());
        //添加订单号
        t = points[x2, y2].transform.GetComponentsInChildren<TextMesh>();
        t[0].text = System.String.Format("{0}", id);
        t[1].text = System.String.Format("{0}", id);

    }
}
