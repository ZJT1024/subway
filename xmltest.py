import xml.etree.cElementTree as ET
import subwaystation
import tkinter as tk
import codecs
from tkinter import messagebox
import time

# 全局变量
# 线路树，储存线路是否是环线、车站名字、换乘车站名字、颜色
line_dic = {}
# 站点链表，其中每个结点储存站点的所有信息：包括坐标、名字、所属的路线等
station_list = []
# 换乘车站链表，其中每个结点储存换成车站的所有信息：包括坐标、名字、所属的路线
transfer_station_list = []
# 车站和UI中的标签对应的链表，其中每个元素组成为（车站，标签）
station_label_list = []

def decode_xml(path):
    tree = ET.parse(path)
    map = tree.getroot()
    # 长宽记录
    w = int(map.find('w').text)
    h = int(map.find('h').text)
    linecnt = 0
    # 遍历路线
    loop_line = ['Line 2', 'Line 4', "Line 10"]
    special_cnt = 0
    for line in map.findall('line'):
        # 标记北京线路的个数
        linecnt += 1
        # 遍历车站
        lname = line.find('lname').text
        color = line.find('color').text
        # print(color)
        line_dic[lname] = {}
        station_cnt = 0
        is_loop = False
        if lname == 'Line 2' or lname == 'Line 4' or lname == 'Line 10':
            is_loop = True
        line_dic[lname]['is_loop'] = is_loop
        line_dic[lname]['transfer_station_name_list'] = []
        line_dic[lname]['station_name_list'] = []
        color = str(color)
        color = '#' + color.upper()
        line_dic[lname]['color'] = color

        for station in line:
            # 标记为是否为环线
            if station.tag != 'station':
                continue
            # 具体的结果
            station_cnt += 1
            staid = station.find('staid').text
            name = station.find('name').text
            sname = station.find('sname').text
            sname = str(sname)
            sname = sname.strip().split()
            sname = sname[len(sname) - 1]
            # 等比例缩小坐标
            mapx = float(station.find('mapx').text) / 2
            mapy = float(station.find('mapy').text) / 2
            pos = int(station.find('pos').text)
            lat = float(station.find('lat').text)
            lng = float(station.find('lng').text)
            # 如果station_line_cnt中存在sname则说明此站肯定是换乘站
            # print(lname, staid, name, sname, mapx, mapy, pos, lat, lng)

            is_transfer = False
            for tmp_station in station_list:
                if tmp_station.sname == sname:
                    is_transfer = True
                    tmp_station.add_lname(lname)
                    transfer_station_list.append(tmp_station)
                    break
            if not is_transfer:
                tmp_station = subwaystation.station(staid, name, sname, mapx, mapy, pos, lat, lng, lname, station_cnt)
                station_list.append(tmp_station)
            line_dic[lname]['station_name_list'].append(sname)

    print()
    for tmp_station in station_list:
        if len(tmp_station.lname_list) > 1:
            transfer_station_list.append(tmp_station)

    print()

    for tmp_station in transfer_station_list:
        tmp_lname_list = tmp_station.lname_list
        sname = tmp_station.sname
        for tmp_lname in tmp_lname_list:
            if sname not in line_dic[tmp_lname]['transfer_station_name_list']:
                line_dic[tmp_lname]['transfer_station_name_list'].append(sname)
            else:
                continue
    return
xml_path = 'stations_v5.xml'
decode_xml(xml_path)

# UI部分，包括UI部分的所有插件
# 缺少Tkinter下MVC的配置资料，故解析和UI有些杂糅
window = tk.Tk()
window.title("subway MAP UI")
window.geometry('1200x1000')

# 寻找最短路线的排版
e1 = tk.Entry(window)
e2 = tk.Entry(window)
l1 = tk.Label(window, text="起始站", font=('Aerial', 15))
l1.place(x=0, y=0)
l2 = tk.Label(window, text="终点站",font=('Aerial', 15))
l2.place(x=0, y = 30)
e1.place(x=50,y = 0)
e2.place(x=50,y = 30)
# 根据题目要求查找从某个起点车站到某个终点车站的路线
def find_a_way_by_cost():
    # 按站寻路为0
    flag = 0
    for (station, l) in station_label_list:
        lname_list = station.lname_list
        if len(lname_list) > 1:
            color = 'orange'
        else:
            lname = station.lname_list[0]
            color = line_dic[lname]['color']
        l['bg'] = color
    start_name = str(e1.get())
    end_name = str(e2.get())

    #########
    flag = False
    for tmp_station in station_list:
        if tmp_station.sname == start_name or tmp_station.sname == end_name:
            flag = True
            break
    if flag == False:
        messagebox.showinfo(title="WRONG!", message="check your input!")
        return
    ###########



    ##########

    # python 调用C++
    ##########

    ##########
    # python对文件进行处理
    tmp_sname_list = []
    input_path = 'station_order.txt'
    input_file = codecs.open(input_path, 'r', 'utf-8')
    i = 0
    output = ''
    for line in input_file.readlines():
        output += line
        if i == 0:
            i += 1
            continue
        info = line.strip().split()
        tmp_sname_list.append(info[0])

    # 动态显示线路
    for tmp_sname in tmp_sname_list:
        for (tmp_station, l) in station_label_list:
            if tmp_sname == tmp_station.sname:
                l['bg'] = 'white'
                time.sleep(.200)
                l.update()
                break

    # 更改右侧的结果
    T.delete(0.0,tk.END)
    T.insert('insert', output)
button = tk.Button(window, text="按站寻路", bg='white', width=10, height=2, command=find_a_way_by_cost)
button.place(x=0, y=60)
# 后期得更改button1的函数
def find_a_way_by_effort():
    pass
button1 = tk.Button(window, text="按精力寻路", bg='white', width=10, height=2, command=find_a_way_by_effort)
button1.place(x=120, y=60)

# 右侧文本框的排版
T = tk.Text(window, height=50, width=30)
T.place(x=1000, y=0)
T.insert('insert',"你好！")

# 按照线路名寻找的排版
e3 = tk.Entry(window)
e3.place(x=0, y=130)
l3 = tk.Label(window, text="输入线路名",font=('Aerial', 15))
l3.place(x=0, y = 100)

# 根据题目要求，输入线路名字，可动态显示此站点中所有车站，在UI界面上显示，
def show_line():
    # 颜色复原
    for (station, l) in station_label_list:
        lname_list = station.lname_list
        if len(lname_list) > 1:
            color = 'orange'
        else:
            lname = station.lname_list[0]
            color = line_dic[lname]['color']
        l['bg'] = color
    line_name = e3.get()
    if line_name not in line_dic.keys():
        messagebox.showinfo(title="WRONG!", message="line_name should in Line 1\Line 2\Line 4/Daxing\Line 5\Line 8\Line 9\Line 10\Line 13\Line 15\Batong Line\Changping Line\Yizhuang Line\Fangshan Line\Airport Express")
        return

    # 显示颜色
    tmp_sname_list = []
    for sname in line_dic[line_name]['station_name_list']:
        tmp_sname_list.append(sname)
    output = ''
    output += line_name
    output += '\n'
    for tmp_sname in tmp_sname_list:
        output += tmp_sname
        output += '\n'
    T.delete(0.0, tk.END)
    T.insert('insert', output)
    T.update()
    for sname in line_dic[line_name]['station_name_list']:
        for (tmp_station, l) in station_label_list:
            if tmp_station.sname == sname:
                l['bg'] = 'white'
                time.sleep(.300)
                l.update()
    # 颜色复原
    time.sleep(3)
    for (station, l) in station_label_list:
        lname_list = station.lname_list
        if len(lname_list) > 1:
            color = 'orange'
        else:
            lname = station.lname_list[0]
            color = line_dic[lname]['color']
        l['bg'] = color
    for (station, l) in station_label_list:
        l.update()
    return
button2 = tk.Button(window, text="显示路线", bg='white', width=15, height=2, command=show_line)
button2.place(x=0, y=160)

# 创建标签，其中每一个标签都是地铁上的一个站
for tmp_station in station_list:
    text = tmp_station.sname
    mapx = tmp_station.mapx
    mapy = tmp_station.mapy
    lname_list = tmp_station.lname_list
    if len(lname_list) > 1:
        color = 'orange'
    else:
        lname = lname_list[0]
        color = line_dic[lname]['color']
    l = tk.Label(window, text=text, bg= color, font=('Aerial', 5))
    l.place(x=mapx, y=mapy)
    station_label_list.append((tmp_station, l))

# 窗口的循环
window.mainloop()
