import xml.etree.cElementTree as ET
import subwayline
import subwaymap
import subwaystation
import tkinter as tk
import codecs
from tkinter import messagebox
import time



tree = ET.parse('stations_v5.xml')
map = tree.getroot()
# 长宽记录
w = int(map.find('w').text)
h = int(map.find('h').text)
station_line_cnt = {}
transfer_station_list = []
linecnt = 0
# 遍历路线
station_list = []
line_dic = {}
loop_line = ['Line 2', 'Line 4', "Line 10"]
special_cnt = 0
station_sname_lname_list = {}
for line in map.findall('line'):
    # 标记北京线路的个数
    linecnt += 1
    # 遍历车站
    lname = line.find('lname').text
    color = line.find('color').text
    print(color)
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
        sname = sname[len(sname)-1]
        mapx = float(station.find('mapx').text)/2
        mapy = float(station.find('mapy').text)/2
        pos = int(station.find('pos').text)
        lat = float(station.find('lat').text)
        lng = float(station.find('lng').text)
        # 如果station_line_cnt中存在sname则说明此站肯定是换乘站
        print(lname, staid, name, sname, mapx, mapy, pos, lat, lng)

        # 找一个西直门验证想法，会不会牵一发而动全身？
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

for tmp_station in station_list:
    station_sname_lname_list[tmp_station.sname] = tmp_station.lname_list

lname_jud = {}
for key in line_dic:
    lname_jud[key] = False


# 传的话，只传line_dic即可
# 站的信息也得传！有站的信息才能绘图
station_label_list = []

# 要包含起点站的名字，终点站的名字，现在站的名字，当前花费，最小花费，当前线路
# 当前最短路径
ans_sname_path = []
# 最短路径的集合
ans_sname_path_list = []
# line_dic存放所有的信息
print("---------------------")
for station in transfer_station_list:
    print(station.sname)
print("---------------------")


# 寻找路径
def search(start_name, end_name, now_name,cost,min_cost):
    is_same_line = False
    # 看看是否在同一条线上
    lname = ''
    for lname1 in station_sname_lname_list[now_name]:
        for lname2 in station_sname_lname_list[end_name]:
            if lname1 == lname2:
                lname = lname1
                is_same_line = True

    # tmp_cost 是从当前车站到终点站或下一个车站的集合
    tmp_cost = 0
    # dir 标记正向还是负向，如果dir = True则为正向，否则为负向
    dir = True
    if is_same_line:
        pos1 = 0
        pos2 = 0
        # 如果现在的站和终点站在一条路线上
        # 标记一下正向和负向
        # 默认标记为正向
        for i in range(0, len(line_dic[lname]['station_name_list'])):
            if line_dic[lname]['station_name_list'][i] == now_name:
                pos1 = i
            if line_dic[lname]['station_name_list'][i] == end_name:
                pos2 = i
        if pos1 > pos2:
            flag = False
        # 如果是环线
        if line_dic[lname]['is_loop']:
            # 从开始到结束
            tmp_cost1 = abs(pos2-pos1)+1
            tmp_cost2 = len(line_dic[lname]['station_name_list'])-abs(pos2-pos1)+1
            if tmp_cost1 < tmp_cost2:
                tmp_cost = tmp_cost1
                if flag:
                    dir = True
                else:
                    dir = False
            else:
                tmp_cost = tmp_cost2
                if flag:
                    dir = False
                else:
                    dir = True
        else:
            tmp_cost = abs(pos2-pos1)+1
            if flag:
                dir = True
            else:
                dir = False
        if tmp_cost + cost < min_cost:
            cost += tmp_cost


    else:
        pass

subway_info = "subway_info.txt"
line_info = "line_info.txt"
subway_text = codecs.open(subway_info, 'w', 'utf-8')
for station in station_list:
    subway_text.write(station.sname + "\n")
    for lname in station.lname_list:
        subway_text.write(lname + "\\")
    subway_text.write("\n")
    subway_text.write("\n")
subway_text.close()

line_text = codecs.open(line_info, 'w', 'utf-8')
for key in line_dic:
    print(key)
    line_text.write(key + "\n")
    line_text.write(str(line_dic[key]['is_loop'])+"\n")
    for sname in line_dic[key]['transfer_station_name_list']:
        line_text.write(sname)
        line_text.write(' ')
    line_text.write("\n")
    for sname in line_dic[key]['station_name_list']:
        line_text.write(sname)
        line_text.write(" ")
    line_text.write("\n")
    # 最后一个需要换行
    line_text.write("\n")

line_text.close()

window = tk.Tk()
window.title("subway MAP UI")
window.geometry('1000x900')
e1 = tk.Entry(window)
e2 = tk.Entry(window)
l1 = tk.Label(window, text="起始站", font=('Aerial', 15))
l1.place(x=0, y=0)
l2 = tk.Label(window, text="终点站",font=('Aerial', 15))
l2.place(x=0, y = 30)
e1.place(x=50,y = 0)
e2.place(x=50,y = 30)


# UI界面应该和这些界面分开来放
name_path = []

def find_a_way():
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

    print(start_name, end_name)
    name_list = []
    name_list.append(start_name)
    name_list.append(end_name)
    # path_name_list:
    for sname in name_list:
        for (tmp_station, l) in station_label_list:
            if tmp_station.sname == sname:
                l['bg'] = 'green'
                break



button = tk.Button(window, text="寻找最短路径", bg='white', width=15, height=2, command=find_a_way)
button.place(x=0, y=60)
e3 = tk.Entry(window)
e3.place(x=0, y=130)


T = tk.Text(window, height=50, width=30)
T.place(x=1000, y=0)
T.insert('insert',"你好！")
l3 = tk.Label(window, text="输入线路名",font=('Aerial', 15))
l3.place(x=0, y = 100)


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
    flag = []
    for i in range(0,len(station_label_list)):
        flag.append(False)
    tmp_sname_list = []
    print("----------------------------------------")
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


    print("----------------------------------------")

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

button1 = tk.Button(window, text="显示路线", bg='white', width=15, height=2, command=show_line)
button1.place(x=0, y=160)


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



window.mainloop()


