#!/bin/python
import pprint
LUT_FILENAME = 'RaidLut.h'
raides = ["a","ą","b","c","č","d","e","ę","ė","f","g","h","i","į","y","j","k","l","m","n","o","p","r","s","š","t","u","ų","ū","v","z","ž"]
raidesNr = len(raides)
print(raidesNr)
trumpieji_balsiai = ["a","e","i","o","u"]
ilgieji_balsiai = ["ą","ę","ė","į","y","o","ų","ū"]
balsiai = trumpieji_balsiai + ilgieji_balsiai
priebalsiai = ['b', 'c', 'č', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'r', 's', 'š', 't', 'v', 'z', 'ž']

#raidziuNr={'a': 1, 'ą': 2, 'b': 3, 'c': 4, 'č': 5, 'd': 6, 'e': 7, 'ę': 8, 'ė': 9, 'f': 10, 'g': 11, 'h': 12, 'i': 13, 'į': 14, 'y': 15, 'j': 16, 'k': 17, 'l': 18, 'm': 19, 'n': 20, 'o': 21, 'p': 22, 'r': 23, 's': 24, 'š': 25, 't': 26, 'u': 27, 'ų': 28, 'ū': 29, 'v': 30, 'z': 31, 'ž': 32}

#priebalsiai = raides.copy()
#for raide in balsiai:
#    try:
#        priebalsiai.remove(raide)
#    except:
#        pass
#        
#print(priebalsiai)
#lut = [0]*26*26
#print(lut)


#balsiai
#i=1
#for pirma in trumpieji_balsiai:
#    raidBePirmos = trumpieji_balsiai.copy()
#    raidBePirmos.remove(pirma)
#    for antra in raidBePirmos:
#        print(pirma+antra,end=', ')
#        lut[(ord(pirma)-97)*26+(ord(antra)-97)]=i
#        i+=1

#print()
#print(lut)
raidesEnum = list()
raidesEnumDict = dict()
for i,raide in enumerate(raides):
    raidesEnum.append(i)
    raidesEnumDict[raide] = i+1
    
#print(raidesEnum)
print(raidesEnumDict)



#with open("./sounds/skiemenys.txt") as file:
with open("./sounds/raides_ir_skiemenys.txt") as file:
    fileCont=file.read()
    fileCont = fileCont.split('\n')
    print(fileCont)

#skiemLut = [[col for col in range(raidesNr+1)] for row in range(raidesNr+1)]
skiemLut = [([0] * (raidesNr+1)) for row in range(raidesNr+1)] # generate a list of lists


#print(skiemLut)
for i,skiem in enumerate(fileCont):
    print(f"{skiem},{i}")
    if len(skiem)>1:
        pirm = skiem[0]
        antr = skiem[1]
        skiemLut[raidesEnumDict[pirm]][raidesEnumDict[antr]] = i+1
    else:
        if skiem in raidesEnumDict:
            skiemLut[0][raidesEnumDict[skiem]] = i+1
            skiemLut[raidesEnumDict[skiem]][0] = i+1


#print(f'uint16_t RaidLut[{raidesNr+1}][{raidesNr+1}]=',end='')
resultString= f'uint16_t RaidLut[{raidesNr+1}][{raidesNr+1}]='
#print(f'F(',end='')
#print('{',end='')
resultString= resultString+'{'
for i2,row in enumerate(skiemLut):
    #print('{',end='')
    resultString= resultString+'{'
    for i,col in enumerate(row):
        #print(col,end='')
        resultString= resultString+f'{col}'
        if i<raidesNr:
            #print(',',end='')
            resultString= resultString+','
    #print('}',end='')
    resultString= resultString+'}'
    if i2 < raidesNr:
        #print(',',end='')
        resultString= resultString+','
#print('};',end='')
resultString= resultString+'};'
#print(')',end='')
print()
print(resultString)
with open(LUT_FILENAME,'w') as fd:
    fd.write(resultString)

print('')

#pprint.pp(skiemLut)
#raides = ["a","ą","b","c","č","d","e","ę","ė","f","g","h","i","į","y","j","k","l","m","n","o","p","r","s","š","t","u","ų","ū","v","z","ž"]
for i,raide in enumerate(raides): 
    print(f"{raide}:{33+i }:{chr(33+i)}")


