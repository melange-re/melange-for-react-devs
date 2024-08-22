module Voice = SpeechSynthesis.Voice;

[@react.component]
let make = () => {
  let (voices, setVoices) = RR.useStateValue([||]);

  React.useEffect0(() => {
    SpeechSynthesis.getVoices() |> Promise.iter(setVoices);
    None;
  });

  <div>
    <select>
      {voices
       |> Stdlib.Array.map(voice => {
            let uri = voice |> Voice.getUri;
            <option key=uri value=uri>
              {voice |> Voice.getName |> RR.s}
            </option>;
          })
       |> React.array}
    </select>
  </div>;
};
