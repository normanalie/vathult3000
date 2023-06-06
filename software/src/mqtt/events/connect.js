/*
 *  - MQTT "connect" event
 */

const eventLogger = require('../../misc/eventLogger');
const { mqttTopicBase } = require('../misc/mqttConfig.json');

module.exports = (mqttApp) => {

    mqttApp.on('connect', () => {

        eventLogger("MQTT", "i", "MQTT connection established", "");
    
        const mqttTopic = `${mqttTopicBase}/#`
    
        mqttApp.subscribe([mqttTopic], (err) => {
    
            if (err) {
                eventLogger("MQTT", "ERROR!", `Failed to subscribe to topic(s) [${mqttTopic}]`, "");
                throw err;
            }
            eventLogger("MQTT", "i", `Subscribed to topic [${mqttTopic}]`, "");
        });
    
        mqttApp.publish(`${mqttTopicBase}/TEST`, '(Test) MQTT Boot Message', { qos: 0, retain: false }, (error) => {  //Test message
            if (error) {
                eventLogger("MQTT", "ERROR!", `Failed to send test to topic(s) [${mqttTopic}]`, "");
                console.error(error)
            }
        });
    })
}
