import csv

outfits = []
odd_row = True
is_first_row = True

with open('Polyvore_tops_bottoms_with_price_occ_rating.csv', 'rt') as f:
	data = csv.reader(f)
	for row in data:
		if is_first_row == True:
			is_first_row = False
			continue
		p_row = []
		p_row.append(row[0])
		p_row.append(row[1])
		p_row.append(row[2])
		p_row.append(row[3])
		p_row.append(row[4])
		p_row.append(row[5])
		if p_row not in outfits:
			outfits.append(p_row)

file = open("preprocess_data_generated.txt", "w")

file.write(str(len(outfits)) + '\n');

for row in outfits:
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
