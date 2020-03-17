dic = {}


def grandparents(parent_class):
    if parent_class not in dic.keys():
        return []
    new_dic = []
    new_dic += dic[parent_class]
    #print(new_dic)
    if len(new_dic) == 0:  #class have not parents
        return []
    for i in dic[parent_class]:
       # print(i)
       # print("-")
        new_dic += grandparents(i)
        #print(new_dic)
        return new_dic


n = int(input())

for i in range(n):
    k = input()
    l = k.split()
    if len(l) > 1:  # class have parents
        parents = l[2:]
        if l[0] not in dic.keys():  # if class not exist yet
           dic[l[0]] = []
        dic[l[0]] += parents
        # thea add all grandparents:
        for j in parents:

            if j in dic.keys():
                dic[l[0]] += grandparents(j)

        for j in dic:
            for k in dic[j]:
                if k == l[0]:
                    dic[j] += dic[l[0]]
        # for j in dic[l[0]]:
         # print(j)
    else:
        dic[l[0]] = []


print(dic)

n = int(input())
for i in range(n):
    k = input()
    l = k.split()
    if l[0] == l[1]:
        print("Yes")
    else:
        if (l[0] in dic.keys()) and (l[0] in dic[l[1]]):
            print("Yes")
        else:
            print('No')
            #print (dic)

