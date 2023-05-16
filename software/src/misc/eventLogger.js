/* 
 *  - Advanced console.log with date and hour
 */


module.exports = (source, type, message, borderCharSource) => {

    const currentDate = new Date;
    const year = `${currentDate.getFullYear()}`;
    const months = `${(currentDate.getMonth()+1) < 10 ? "0" : ""}${currentDate.getMonth()+1}`;
    const date = `${currentDate.getDate() < 10 ? "0" : ""}${currentDate.getDate()}`;
    const hours = `${currentDate.getHours() < 10 ? "0" : ""}${currentDate.getHours()}`;
    const minutes = `${currentDate.getMinutes() < 10 ? "0" : ""}${currentDate.getMinutes()}`;
    const seconds = `${currentDate.getSeconds() < 10 ? "0" : ""}${currentDate.getSeconds()}`;

    console.log(`[${year}/${months}/${date} - ${hours}:${minutes}:${seconds}] ${borderCharSource}(${source})${borderCharSource} [${type}] - ${message}`);
}