/*
 *  - MQTT "message" event 
 */

const eventLogger = require("../../misc/eventLogger");
const { mqttTopicBase } = require('../misc/mqttConfig.json');
const  dbStructure = require('../../misc/db-structure.json');


module.exports = (mqttApp, db) => {

    mqttApp.on('message', (topic, payload) => {

        let message = payload.toString();
        eventLogger("MQTT", "<--", `[${topic}] ${message}`, "");
    
        const devicetype = topic.split('/')[0];
    
        if (devicetype == mqttTopicBase && topic != `${mqttTopicBase}/TEST`) {
            const serialNumber = topic.slice(12);
            message = JSON.parse(message);
    
            
            let sql = `UPDATE ${dbStructure.defaultDB.Vathults.tableName}
                       SET ${dbStructure.defaultDB.Vathults.flow} = '${message.flow}',
                           ${dbStructure.defaultDB.Vathults.waterAlert} = '${message.water_alert}',
                           ${dbStructure.defaultDB.Vathults.input} = '${message.input}',
                           ${dbStructure.defaultDB.Vathults.output1} = '${message.output1}',
                           ${dbStructure.defaultDB.Vathults.output2} = '${message.output2}',
                           ${dbStructure.defaultDB.Vathults.output3} = '${message.output3}',
                           ${dbStructure.defaultDB.Vathults.output4} = '${message.output4}',
                           ${dbStructure.defaultDB.Vathults.wifiStrength} = '${message.wifi_strength}',
                           ${dbStructure.defaultDB.Vathults.wifiSSID} = '${message.wifi_ssid}',
                           ${dbStructure.defaultDB.Vathults.messages} = '${JSON.stringify(message.messages)}'
                       WHERE ${dbStructure.defaultDB.Vathults.serialNumber} = '${serialNumber}'`;
            
            db.query(sql, (err) => {
                if (err) {
                    eventLogger("MySQL", "ERROR!", `Unable to save Vathults datas (SN: ${serialNumber})`, "");
                    console.log(err);
                } else {
                    eventLogger("MySQL", "i", `Vathults Data successfully registered (SN: ${serialNumber})`, "");
                }
            })
        }
    })
}