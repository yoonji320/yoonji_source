#!/bin/bash

if [[ "$3" == "" ]]; then
	echo ""
	echo "	Usage: sh $0 <result_file> <work_id> <work_desc>"
	echo ""
	exit
fi

DB_HOST=10.10.10.185
DB_USER=nsmart
DB_PASSWD=nsmart@dlcjsrn

RESULT_FILE=$1
WORK_ID=$2
WORK_DESC=$3
FLAG=TRUE

DB_TEMP=./db_temp.tmp
NETCOUNT_TMP=./netcount_tmp.tmp
OUTPUT_TMP=./output_tmp.tmp

while read line
do
	if [[ "$line" == "--------------" ]]; then
		FLAG=FALSE
	fi

	if [[ "${FLAG}" == "TRUE" ]]; then
		echo "$line" >> ${NETCOUNT_TMP}

	elif [[ "${FLAG}" == "FALSE" && "$line" != "--------------" ]]; then
		echo "$line" >> ${OUTPUT_TMP}
	fi
done < ${RESULT_FILE}

### dup_stats_net_dev_count 테이블에 insert ###
/usr/local/mysql/bin/mysql -u${DB_USER} -p${DB_PASSWD} --local-infile=1 adnet_report -h${DB_HOST} -e "LOAD DATA LOCAL INFILE '${NETCOUNT_TMP}' INTO TABLE adnet_report.dup_stats_net_dev_count FIELDS TERMINATED BY ',' (work_id, net_id, dev_count, data_class);"
	rm -f ${NETCOUNT_TMP}

### dup_stats_data 테이블에 insert ###
/usr/local/mysql/bin/mysql -u${DB_USER} -p${DB_PASSWD} --local-infile=1 adnet_report -h${DB_HOST} -e "LOAD DATA LOCAL INFILE '${OUTPUT_TMP}' INTO TABLE adnet_report.dup_stats_data FIELDS TERMINATED BY ',' (work_id, net_id_1, net_id_2, dev_count, data_class);"
	rm -f ${OUTPUT_TMP}

### dup_stats_network 테이블에 insert ###
/usr/local/mysql/bin/mysql -u${DB_USER} -p${DB_PASSWD} --local-infile=1 adnet -h${DB_HOST} -N -e "SELECT adnetwork_id, adnetwork_title FROM ax_adnetwork;" > ${DB_TEMP}

/usr/local/mysql/bin/mysql -u${DB_USER} -p${DB_PASSWD} --local-infile=1 adnet_report -h${DB_HOST} -N -e "TRUNCATE TABLE dup_stats_network;"

/usr/local/mysql/bin/mysql -u${DB_USER} -p${DB_PASSWD} --local-infile=1 adnet_report -h${DB_HOST} -e "LOAD DATA LOCAL INFILE '${DB_TEMP}' INTO TABLE adnet_report.dup_stats_network FIELDS TERMINATED BY '\t' (net_id, net_name, reg_date);"

### dup_stats_work 테이블에 insert ###
/usr/local/mysql/bin/mysql -u${DB_USER} -p${DB_PASSWD} --local-infile=1 adnet_report -h${DB_HOST} -e "INSERT INTO dup_stats_work (work_id, work_desc) VALUES (${WORK_ID}, '${WORK_DESC}');"
