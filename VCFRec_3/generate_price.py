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
        
price_fw = {}
is_first_row = True

with open('price_fw.csv', 'rt') as f:
    data = csv.reader(f)
    for row in data:
        if is_first_row == True:
            is_first_row = False
            continue
        splitted_string = row[3].split('/')
        price_fw[splitted_string[-1]] = row[5]
        
filtered_data = []
is_first_row = True

with open('amazon_outfits_data_5th_feb_2020_updated_with_category_filtered_100.csv', 'rt') as f:
    data = csv.reader(f)
    for row in data:
        if is_first_row == True:
            is_first_row = False
            continue
        if row[11] == '0':
            continue
        filtered_row = []
        item1 = row[2].split('/')
        item2 = row[3].split('/')
        item3 = row[4].split('/')
        if item1[-1] in price_tw and item2[-1] in price_bw and item3[-1] in price_fw:
            occasion = row[10]
            filtered_row.append(item1[-1])
            filtered_row.append(item2[-1])
            filtered_row.append(item3[-1])
            filtered_row.append(price_tw[item1[-1]])
            filtered_row.append(price_bw[item2[-1]])
            filtered_row.append(price_fw[item3[-1]])
            filtered_row.append(occasion)
            filtered_data.append(filtered_row)

with open('outfit_data.csv', mode='w') as csv_file:
    fieldnames = ['tw', 'bw', 'fw', 'price_tw', 'price_bw', 'price_fw', 'occasion']
    writer = csv.writer(csv_file)
    writer.writerow(fieldnames)
    for row in filtered_data:
        writer.writerow(row)