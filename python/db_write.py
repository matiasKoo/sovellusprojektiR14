import mysql.connector
import os
from dotenv import load_dotenv

load_dotenv()  # take environment variables from .env.

mydb = mysql.connector.connect(
  host=os.getenv("HOST"),
  user=os.getenv("USER"),
  password=os.getenv("PASSWORD"),
  database=os.getenv("DATABASE")
)

mycursor = mydb.cursor()

def db_write(suunta,x,y,z):
  sql="INSERT INTO rawdata (groupid,sensorvalue_a,sensorvalue_b,sensorvalue_c,sensorvalue_d) VALUES (14,%s,%s,%s,%s)"
  val=(suunta,x,y,z)

  mycursor.execute(sql,val)

  mydb.commit()

