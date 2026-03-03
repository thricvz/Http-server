const button = document.getElementById("button-1")
button.addEventListener("click", send_request)

function send_request() {
    const text_input = document.getElementById("input-1")
    const msg = text_input.value

    const fetch_result = fetch('http://localhost:8080/owaowa', {
      method : "POST",
      body : msg
    }).then(response => response.json())
    .then(body => console.log(body))
    .catch(error => console.log("had and error " + error))

    return 0;
}
