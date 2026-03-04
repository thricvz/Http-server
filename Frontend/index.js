const button = document.getElementById("button-1")
button.addEventListener("click", send_request)

function send_request() {
    const text_input = document.getElementById("input-1")
    const msg = text_input.value

    const fetch_result = fetch('http://localhost:8080/hello', {
      method : "POST",
      body : msg
    }).then(response => response.json())
    .then(body => add_item_to_list(body["message"]))
    .catch(error => console.log("had and error " + error))

    return 0;
}

function add_item_to_list(message) {
    const list  = document.getElementById("list")
    new_element = document.createElement("li")
    // I AM A COMMUNITY BOP
    new_element.textContent = message
    
    list.appendChild(new_element)
}
