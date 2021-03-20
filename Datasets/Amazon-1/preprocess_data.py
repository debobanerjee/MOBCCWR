import csv

price_tw = {}
is_first_row = True

with open('price_tw.csv', 'rt') as f:
	data = csv.reader(f)
	for row in data:
		if is_first_row == True:
			is_first_row = False
			continue
		splitted_string = row[3].split('/')
		price_tw[splitted_string[-1]] = row[5]


price_bw = {}
is_first_row = True

with open('price_bw.csv', 'rt') as f:
	data = csv.reader(f)
	for row in data:
		if is_first_row == True:
			is_first_row = False
			continue
		splitted_string = row[3].split('/')
		price_bw[splitted_string[-1]] = row[5]

filtered_data = []
is_first_row = True

usedTW = []
usedBW = []
usedO = []

uselessOutfits = 0

with open('Amazon_Old_Pos_Top_Bottom_Pairs.csv', 'rt') as f:
	data = csv.reader(f)
	for row in data:
		if is_first_row == True:
			is_first_row = False
			continue
		filtered_row = []
		item1 = row[1].split('/')
		item2 = row[2].split('/')
		if item1[-1] in price_tw and item2[-1] in price_bw :
			occasion = row[3]
			filtered_row.append(item1[-1])
			filtered_row.append(item2[-1])
			filtered_row.append(price_tw[item1[-1]])
			filtered_row.append(price_bw[item2[-1]])
			filtered_row.append(occasion)
			filtered_row.append(row[4])
			filtered_data.append(filtered_row)
			if item1[-1] not in usedTW:
				usedTW.append(item1[-1])
			if item2[-1] not in usedBW:
				usedBW.append(item2[-1])
			o = []
			o.append(item1[-1])
			o.append(item2[-1])
			if o not in usedO:
				usedO.append(o)
		else:
			uselessOutfits += 1

file = open("preprocess_data_generated.txt", "w")

file.write(str(len(filtered_data)) + '\n');

for row in filtered_data:
	file.write(row[0] + ' ')
	file.write(row[1] + ' ')
	file.write(row[2] + ' ')
	file.write(row[3] + ' ')
	occasions = row[4].split(',')
	file.write(str(len(occasions)) + ' ')
	for u in occasions:
		file.write(u + ' ')
	file.write(row[5])
	file.write('\n')
file.close()

print(uselessOutfits)
print(len(usedTW))
print(len(usedBW))
print(len(usedO))
