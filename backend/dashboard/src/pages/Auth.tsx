import { useState } from "react";
import { useNavigate } from "react-router-dom";

const Auth: React.FC = () => {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const navigate = useNavigate();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError("");

    try {
      const res = await fetch("http://localhost:13000/auth/login", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ email, password }),
      });

      const data = await res.json();

      if (!res.ok) {
        throw new Error(data.message || "Login failed");
      }

      // Spara token och role i localStorage
      localStorage.setItem("token", data.token);
      localStorage.setItem("role", data.role);

      // Navigera baserat på rollen
      if (data.role === "admin") {
        navigate("/dashboard");
      } else if (data.role === "user") {
        navigate("/userdashboard");
      } else if (data.role === "moderator") {
        navigate("/moderatorpage");
      } else {
        navigate("/login");
      }
    } catch (err: any) {
      setError(err.message);
    }
  };


  return (
    <main className="flex items-center justify-center min-h-screen bg-[#10302B] p-6">
      <div className="bg-white/10 rounded-lg p-8 w-full max-w-md backdrop-blur-sm">
        <div className="flex justify-center mb-5">
          <img src="/pax.logo.png" alt="Pax logo" className="w-32" />
        </div>

        <h1 className="text-2xl font-bold text-white">Logga in</h1>
        <form className="mt-4" onSubmit={handleSubmit}>
          <div className="mb-4">
            <label htmlFor="username" className="block text-sm font-medium text-white">
              Användarnamn
            </label>
            <input
              type="email"
              id="email"
              className="mt-1 block w-full p-2 rounded-md border-gray-300 shadow-sm focus:ring-indigo-500 focus:border-[#10302B] sm:text-sm"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              required
            />
          </div>

          <div className="mb-4">
            <label htmlFor="password" className="block text-sm font-medium text-white">
              Lösenord
            </label>
            <input
              type="password"
              id="password"
              className="mt-1 block w-full p-2 rounded-md border-gray-300 shadow-sm focus:ring-indigo-500 focus:border-[#10302B] sm:text-sm"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              required
            />
          </div>

          {error && <p className="text-red-500 text-sm mb-3">{error}</p>}

          <button
            type="submit"
            className="w-full bg-[#7DBA6A] text-black font-semibold py-2 px-4 rounded-md hover:bg-[#B5DA87] transition"
          >
            Logga in
          </button>
        </form>
      </div>
    </main>
  );
};

export default Auth;
