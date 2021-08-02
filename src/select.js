const selectBodyNode = document.querySelector(".c-select__body");
const selectNode = document.querySelector(".c-select");
let textAreaNode = document.querySelector(".c-antiPlagiarizm__textarea");
const btnSendNode = document.querySelector(".c-antiPlagiarizm__button");
const WORDS_NUMBER = 2;

Array.from(selectBodyNode.children).forEach((prop) => {
  prop.style.backgroundColor = "rgba(108, 117, 137, 0.6)";
  prop.style.borderRadius = "0.625rem";
  prop.style.cursor = "not-allowed";

  prop.addEventListener("click", (e) => {
    e.preventDefault();
    selectBodyNode.classList.toggle("is-active");
  });
});

selectNode.addEventListener("click", () => {
  selectBodyNode.classList.toggle("is-active");
});

textAreaNode.addEventListener("keyup", (e) => {
  let arrayWords = e.target.value.split(" ");
  let counterWords = arrayWords.length;

  if (arrayWords[WORDS_NUMBER] !== "") {
    if (counterWords > WORDS_NUMBER) {
      btnSendNode.disabled = false;
      btnSendNode.title = "";
    } else {
      btnSendNode.disabled = true;
      btnSendNode.title = "Enter at least 3 words!";
    }
  }

  if (counterWords > WORDS_NUMBER && e.code === "Enter") {
    btnSendNode.disabled = false;
    btnSendNode.title = "";
    btnSendNode.click();
  }
});
