const BASE_URL = "https://paxdb.vercel.app";

export const loginWithApi = async (email, password) => {
    try {
        const response = await fetch(`${BASE_URL}/auth/login`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ email, password })
        });
        if (!response.ok) throw new Error("Inloggningen misslyckades");
        // return await response.json(); // Innehåller tokens
        const data = await response.json();
        console.log(" JWT-token:", data.token); // logga token här (eller accessToken om det heter så)
        console.log(data);
        return data;
    } catch (error) {
        console.error("Login error", error);
        return null;
    }
};