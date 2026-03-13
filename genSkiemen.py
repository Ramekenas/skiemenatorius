#!/bin/python
#raides = ["a","ą","b","c","č","d","e","ę","ė","f","g","h","i","į","y","j","k","l","m","n","o","p","r","s","š","t","u","ų","ū","v","z","ž"]
trumpieji_balsiai = ["a","e","i","o","u"]
ilgieji_balsiai = ["ą","ę","ė","į","y","o","ų","ū"]
balsiai = trumpieji_balsiai + ilgieji_balsiai
priebalsiai = ['b', 'c', 'č', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'r', 's', 'š', 't', 'v', 'z', 'ž']
#priebalsiai = raides.copy()
#for raide in balsiai:
#    try:
#        priebalsiai.remove(raide)
#    except:
#        pass
#        
#print(priebalsiai)

raides = ["a","b","c","č","d","e","ė","f","g","h","i","j","k","l","m","n","o","p","r","s","š","t","u","v","z","ž"]
#raides = ["a","b","c","č","d","e","ė","f","g","h","i","j","k","l","m","n","o","p","r","s","t","u","v","z","ž"]

#balsiai
i=1
for pirma in raides:
    raidBePirmos = raides.copy()
    raidBePirmos.remove(pirma)
    for antra in raidBePirmos:
        #print(pirma+antra,end=', ')
        print(pirma+antra)
        i+=1
