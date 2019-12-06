import os
from openpyxl import Workbook
wb = Workbook()
ws1 = wb.active
ws1.title = "Checkin"
ws2 = wb.create_sheet("Checkout")
ws2.title = "Checkout"

for i in range(1,13):
	exists = os.path.isfile(str(i)+'.xlsx')
	if exists:
		exit("File"+" "+str(i)+".xlsx"+" "+ "Exist")
	else:
		wb.save(filename = str(i)+'.xlsx')

		
	